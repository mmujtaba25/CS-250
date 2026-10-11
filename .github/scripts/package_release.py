"""Release snapshots requested by one full line: [ZIP] Lab|Assignments N.

Tags and ZIP basenames: <lab|assignment>-<NN>.
Titles: <Lab|Assignment> NN. Leading zeros in N are accepted.
"""

import json
import os
from pathlib import Path
import re
import subprocess
import sys

DIRECTIVE = re.compile(r"^\[ZIP\] (Lab|Assignments) (0*[1-9][0-9]*)$", re.MULTILINE)
ZERO_SHA = "0" * 40


def git(*args):
    return subprocess.check_output(["git", *args], text=True).strip()


def request(commit):
    message = git("show", "-s", "--format=%B", commit)
    matches = DIRECTIVE.findall(message)
    if not matches:
        if "[ZIP]" in message:
            raise ValueError(f"{commit}: expected '[ZIP] Lab N' or '[ZIP] Assignments N' on its own line; N must be positive.")
        return None
    if len(matches) != 1:
        raise ValueError(f"{commit}: only one ZIP directive is allowed per commit.")
    kind, raw_number = matches[0]
    number = int(raw_number)
    if kind == "Lab":
        return f"Labs/Lab {number:02d}", f"Lab {number:02d}", f"lab-{number:02d}"
    return f"Assignments/Assignment {number}", f"Assignment {number:02d}", f"assignment-{number:02d}"


def discover():
    event = json.loads(Path(os.environ["GITHUB_EVENT_PATH"]).read_text())
    before, after = event["before"], event["after"]
    commits = []
    if not event.get("deleted") and after != ZERO_SHA:
        # Read Git history rather than a potentially truncated webhook array.
        # New branches scan reachable history; completed releases are reused.
        if before != ZERO_SHA:
            if subprocess.run(
                ["git", "cat-file", "-e", f"{before}^{{commit}}"],
                stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
            ).returncode:
                subprocess.run(["git", "fetch", "origin", before], check=True)
            revision = f"{before}..{after}"
        else:
            revision = after
        commits = [commit for commit in git("rev-list", "--reverse", revision).splitlines() if request(commit)]
    with Path(os.environ["GITHUB_OUTPUT"]).open("a") as output:
        output.write(f"matrix={json.dumps(commits, separators=(',', ':'))}\n")
    print(f"Found {len(commits)} ZIP request(s).")


def package(commit, output):
    commit = git("rev-parse", "--verify", f"{commit}^{{commit}}")
    target = request(commit)
    if target is None:
        raise ValueError(f"{commit}: no ZIP directive found.")
    directory, label, slug = target
    if git("cat-file", "-t", f"{commit}:{directory}") != "tree":
        raise ValueError(f"{commit}: {directory} must be a directory.")
    tag, title = slug, label
    output.mkdir(parents=True, exist_ok=True)
    archive = output / f"{tag}.zip"
    # Archiving the commit with a pathspec preserves its fixed timestamps.
    # The archive contains the selected repository directory and tracked files.
    subprocess.run([
        "git", "archive", "--format=zip", f"--output={archive.resolve()}",
        commit, directory,
    ], check=True)
    previous = None
    for ancestor in git("rev-list", "--topo-order", commit).splitlines()[1:]:
        try:
            candidate = request(ancestor)
        except ValueError:
            continue
        if candidate and candidate[2] == slug:
            previous = ancestor
            break
    revision = f"{previous}..{commit}" if previous else commit
    changes = git("log", "--reverse", "--format=%H%x09%s", revision, "--", directory).splitlines()
    url = f"{os.environ.get('GITHUB_SERVER_URL', 'https://github.com')}/{os.environ['GITHUB_REPOSITORY']}"
    notes = [
        f"# {title}", "", f"- Directory: `{directory}`",
        f"- Commit: [`{commit}`]({url}/commit/{commit})",
        f"- Archive: `{archive.name}`", "", "## Changelog", "",
    ]
    if previous:
        notes.extend([f"Changes since the previous ZIP request: [compare]({url}/compare/{previous}...{commit}).", ""])
    else:
        notes.extend(["Initial ZIP release; includes this directory's history.", ""])
    for change in changes:
        sha, subject = change.split("\t", 1)
        subject = re.sub(r"([\\`*_{\[\]<>])", r"\\\1", subject)
        notes.append(f"- {subject} ([`{sha[:12]}`]({url}/commit/{sha}))")
    if not changes:
        notes.append("- No directory changes since the previous ZIP request.")
    notes_file = output / f"{tag}.md"
    notes_file.write_text("\n".join(notes) + "\n")
    return commit, tag, title, archive, notes_file


def ensure_tag(commit, tag, notes):
    """Update the stable annotated tag, without letting an old retry roll it back."""
    ref = f"refs/tags/{tag}"
    remote = subprocess.run(
        ["git", "ls-remote", "--exit-code", "origin", ref, f"{ref}^{{}}"],
        text=True, capture_output=True,
    )
    expected = ""
    if remote.returncode == 0:
        refs = dict(line.split("\t", 1)[::-1] for line in remote.stdout.splitlines())
        # Annotated tags resolve through their peeled ref; lightweight tags
        # from earlier workflow versions point directly to the commit.
        target = refs.get(f"{ref}^{{}}", refs.get(ref))
        expected = refs[ref]
        if target == commit:
            print(f"Verified existing tag {tag} at {commit}.")
            return True
        subprocess.run(["git", "fetch", "origin", ref], check=True)
        ancestry = subprocess.run(["git", "merge-base", "--is-ancestor", commit, target])
        if ancestry.returncode == 0:
            print(f"Skipping older request {commit}; {tag} already points to {target}.")
            return False
        if ancestry.returncode != 1:
            raise subprocess.CalledProcessError(ancestry.returncode, ancestry.args)
    elif remote.returncode != 2:  # ls-remote returns 2 when no ref matches.
        raise subprocess.CalledProcessError(
            remote.returncode, remote.args, output=remote.stdout, stderr=remote.stderr
        )

    environment = os.environ.copy()
    environment["GIT_COMMITTER_DATE"] = git("show", "-s", "--format=%cI", commit)
    subprocess.run([
        "git", "-c", "user.name=github-actions[bot]",
        "-c", "user.email=41898282+github-actions[bot]@users.noreply.github.com",
        "tag", "--force", "--annotate", tag, commit, "--cleanup=verbatim", "--file", str(notes),
    ], check=True, env=environment)
    subprocess.run([
        "git", "push", f"--force-with-lease={ref}:{expected}", "origin", f"{ref}:{ref}",
    ], check=True)
    print(f"Published tag {tag} at {commit}.")
    return True


def publish_commit(commit):
    commit, tag, title, archive, notes = package(commit, Path("release-assets"))
    if not ensure_tag(commit, tag, notes):
        return
    # Publishing is serialized so tag, release notes, and archive stay aligned.
    existing = subprocess.run(["gh", "release", "view", tag], stdout=subprocess.DEVNULL).returncode == 0
    if existing:
        subprocess.run(["gh", "release", "edit", tag, "--title", title, "--notes-file", str(notes)], check=True)
    else:
        subprocess.run([
            "gh", "release", "create", tag, "--verify-tag", "--draft",
            "--title", title, "--notes-file", str(notes), "--latest=false",
        ], check=True)
    subprocess.run(["gh", "release", "upload", tag, str(archive), "--clobber"], check=True)
    # Explicitly publish after upload, including drafts left by earlier runs.
    subprocess.run([
        "gh", "release", "edit", tag, "--draft=false", "--prerelease=false",
        "--latest=false", "--verify-tag",
    ], check=True)
    state = json.loads(subprocess.check_output([
        "gh", "release", "view", tag, "--json", "isDraft,isPrerelease,url",
    ], text=True))
    if state["isDraft"] or state["isPrerelease"]:
        raise ValueError(f"Release {tag} was not published as a regular release.")
    print(f"Published release: {state['url']}")


def publish():
    for commit in json.loads(os.environ["RELEASE_COMMITS"]):
        publish_commit(commit)


if __name__ == "__main__":
    try:
        {"discover": discover, "publish": publish}[sys.argv[1]]()
    except (ValueError, subprocess.CalledProcessError) as error:
        print(f"ZIP release failed: {error}", file=sys.stderr)
        sys.exit(1)
