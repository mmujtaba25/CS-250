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
| Release title | `Lab 05 (<SHA12>)` | `Assignment 02 (<SHA12>)` |
| Tag | `lab-05-<SHA40>` | `assignment-02-<SHA40>` |
| ZIP filename | `lab-05-<SHA40>.zip` | `assignment-02-<SHA40>.zip` |

The full commit SHA makes each release unique and its name predictable. Repeated
runs update the same release and replace the same asset. Releases are not marked
as the repository's latest release because branch execution order can vary.

Before publishing a release, the workflow creates and pushes an annotated Git
tag at the exact ZIP request commit. Its annotation contains the release notes,
and its tagger timestamp matches that commit's timestamp. Release creation
requires the remote tag to exist. Retries verify existing tags and fail if a tag
points to a different commit; tags are never moved or force-pushed. Lightweight
tags created by the earlier workflow remain valid and are reused.

ZIPs contain tracked files from the requested commit, preserving the repository
directory structure. Git archive export attributes apply. Files added in later
commits and local untracked files are excluded. Entry order and timestamps come
from Git, making repeated archives of the same commit deterministic.

Release notes include the directory, commit link, archive name, and chronological
commits affecting that directory since the nearest prior matching ZIP directive
in its ancestry. The first release includes the directory's full commit history.
The baseline is the previous request, even if its publishing run has not finished.

The workflow supports at most 256 matching commits per push, GitHub's matrix limit.
It replaces the previous manual directory/tag inputs with commit-derived names.
