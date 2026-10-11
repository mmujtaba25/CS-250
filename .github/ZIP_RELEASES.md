# ZIP releases

Push to any branch with exactly one directive on its own line in a commit message:

```text
[ZIP] Lab 5
```

```text
[ZIP] Assignments 2
```

Numbers must be positive; leading zeros are accepted. Additional description
can follow on separate lines. Invalid or multiple directives fail the workflow.
Commits without `[ZIP]` are ignored. Every matching commit in a push is processed,
including matching commits before the branch tip. Tag pushes and branch deletions
do not publish releases. A newly created branch scans its reachable history.

| Field | Lab 5 | Assignments 2 |
| --- | --- | --- |
| Repository directory | `Labs/Lab 05` | `Assignments/Assignment 2` |
| Release title | `Lab 05` | `Assignment 02` |
| Tag | `lab-05` | `assignment-02` |
| ZIP filename | `lab-05.zip` | `assignment-02.zip` |

Each lab or assignment has one stable release URL and ZIP filename. Later ZIP
requests update that release, replace its ZIP, and move its annotated tag to the
requested commit. The full commit SHA remains in the release notes for traceability.
Requests in a push are processed in order; publishing jobs are serialized. An
older request is skipped if the tag already points to one of its descendants.
For unrelated branch histories, the last processed request wins. Releases are
not marked as the repository's latest release.

New releases are prepared as drafts while the ZIP uploads, then explicitly
published as regular releases. Existing drafts are also published after upload.
The workflow verifies the final draft and prerelease flags; it fails if either
is still set. If an upload fails, rerunning the updated workflow completes it
and publishes the draft.

The tag annotation contains the release notes and uses the commit's timestamp.
Tag updates use a force-with-lease check so an unexpected concurrent change
fails the push. Release creation requires the remote tag to exist. Existing
lightweight tags are reused when they already point to the requested commit.

Lab 05 links:

- Release: https://github.com/mmujtaba25/CS-250/releases/tag/lab-05
- ZIP: https://github.com/mmujtaba25/CS-250/releases/download/lab-05/lab-05.zip

ZIPs contain tracked files from the requested commit, preserving the repository
directory structure. Git archive export attributes apply. Files added in later
commits and local untracked files are excluded. Entry order and timestamps come
from Git, making repeated archives of the same commit deterministic.

Release notes include the directory, commit link, archive name, and chronological
commits affecting that directory since the nearest prior matching ZIP directive
in its ancestry. The first release includes the directory's full commit history.
The baseline is the previous request, even if its publishing run has not finished.

It replaces the previous manual directory/tag inputs with commit-derived names.
