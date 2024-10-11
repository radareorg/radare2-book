## Project Versioning

Radare2 includes a built-in version control system that operates similarly to Git, offering project versioning under the command `ravc2` (or its API counterpart, `rvc`). This allows users to manage their project history, track changes, revert to previous versions, and more. Radare2's project versioning system integrates seamlessly with Git, allowing you to choose between Radare2's versioning system or wrapping Git under the same interface.

### Why Use Version Control in Radare2?

Versioning projects can be especially useful when collaborating with others or tracking complex analyses. Since Radare2 projects are essentially scripts, they are easy to read, review, and compare between different versions. This makes version control a great tool for managing your analysis history and for collaborative work.

### Basic Usage of `ravc2`

Below is the basic syntax for using `ravc2`. This command offers a range of actions to initialize repositories, manage branches, commit changes, and view project history.

```console
$ ravc2 -h
Usage: ravc2 [-qvh] [action] [args ...]
Flags:
 -q       quiet mode
 -v       show version
 -h       display this help message
 -j       json output
Actions:
 init     [git | rvc]          initialize a repository with the given vc
 branch   [name]               if a name is provided, create a branch with that name, otherwise list branches
 commit   [message] [files...] commit the files with the message
 checkout [branch]             switch to the given branch
 status                        print a status message
 reset                         remove all uncommitted changes
 log                           print all commits
 RAVC2_USER=[n]                override cfg.user value to author commit
```

### Key `ravc2` Actions Explained

#### 1. Initializing a Repository

To start versioning your project, you need to initialize a repository. You can choose between `rvc` (Radare2's native version control) or `git`.

```console
$ ravc2 init [git | rvc]
```

* `git`: Initializes a Git repository inside your project directory.
* `rvc`: Initializes an `rvc` repository, Radare2’s internal version control.

#### 2. Creating and Listing Branches

You can create a new branch or list existing branches using the `branch` command:

```console
$ ravc2 branch [name]
```

If a branch name is provided, a new branch with that name will be created. If no name is provided, Radare2 will list all the branches in the repository.

#### 3. Committing Changes

Once you've made changes to your project, you can commit them with a message. You can specify which files to commit, or leave it empty to commit all changes.

```console
$ ravc2 commit "Your commit message" [files...]
```

#### 4. Checking Out Branches

Switch to a different branch using the `checkout` command:

```console
$ ravc2 checkout [branch]
```

This will update your project to the state of the selected branch.

#### 5. Viewing Project Status

To see the current status of your project, such as which files have been modified or staged for commit, use the `status` command:

```console
$ ravc2 status
```

#### 6. Resetting Uncommitted Changes

If you want to discard all uncommitted changes and revert to the last committed state, use the `reset` command:

```console
$ ravc2 reset
```

#### 7. Viewing Commit History

To view the full commit history of your project, use the `log` command. This will list all commits made to the project:

```console
$ ravc2 log
```

### Customizing Author Information

You can override the author name of your commits using the `RAVC2_USER` environment variable. This is useful when working on shared projects or if you need to commit as a different user.

```console
RAVC2_USER=[name]
```

### Conclusion

Radare2's versioning system with `ravc2` allows for efficient project management, enabling users to track progress, collaborate, and ensure that their analysis history is well-documented. Whether you use `rvc` or integrate with Git, version control in Radare2 provides flexibility and powerful tools for maintaining analysis continuity.

