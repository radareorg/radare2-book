## Using Projects

Projects in Radare2 cover the way to save and restore metadata for your analysis sessions. A project stores all session data such as commands, analysis results, and optional binaries. This makes it easy to resume work later without needing to repeat previous steps. When a project is saved, Radare2 creates a dedicated directory with the project name containing:

* An `.r2` script: A file that stores all commands needed to recreate the session.
* Optional **sdb** files: These store structured data, including flags, types, and other annotations.
* A copy of the binary file you are analyzing (optional when `-e prj.files` is set).

You can start working on a binary and then save your progress by assigning a project name using the `P` command. Below is a step-by-step guide on how to manage projects in Radare2.

### Warning

Projects has been always a topic of concern in r2land. The reason is because main devs don't use them and taking into account the whole perspective of the problems (see challenges section) when dealing with that in a tool as powerful and flexible as r2 it makes it hard to cope with every single aspect and we never wanted to announce it as a stable or desirable feature.

Taking into account that users known and care about <10% of the whole set of features of r2 it should be possible to create a simplified version of projects to cover only for those cases. We encourage everyone to read this chapter carefully, test all the related capabilities, submit patches and suggest ideas for improving them. Thanks in advance!

### In Short

Use the `r2 -p` flag to list and select the project you want to open (if that was previously created).

When you are analyzing a program and want to save the project use the `P` command to do that like this: `P+projectname`.

Projects are kept in `dir.projects` in your home directory by default and structure the data in directories and files, containing r2 scripts and other companion files.

Note that if you want to export or import the project you must use the `Pz` command, which creates a zip file in the current directory or given path, which makes it ideal to send it to anyone else to continue working on that. Project metadata is versioned using ravc2 which makes use of `git` if available in the system, which lets you have better control on the changes over time.

### Configuring the Project Directory

By default, Radare2 saves projects in the following directory:

```console
[0x00000000]> e dir.projects
~/.local/share/radare2/projects
[0x00000000]>
```

This directory can be changed using the `dir.projects` configuration setting if you want to store your projects elsewhere.

### Project Commands

You can manage projects using the `P` command in Radare2. Below is a breakdown of the available commands for project management.

```console
[0x00000000]> P?
Usage: P[?.+-*cdilnsS] [file]  Project management
| P [file]          open project (formerly Po)
| P.                show current loaded project (see prj.name)
| P+ [name]         save project (same as Ps, but doesn't check for changes)
| P- [name]         delete project
| P*                print project script as r2 commands
| P!([cmd])         open a shell or run command in the project directory
| Pc                close current project
| Pd [N]            diff Nth commit
| Pi [file]         show project information
| Pl                list all projects
| Pn -              edit current loaded project notes using cfg.editor
| Pn[j]             manage notes associated with the project
| Ps [file]         save project (see dir.projects)
| PS [file]         save script file
| PS* [name]        print the project script file (like P*, but requires a project)
| Pz[ie] [zipfile]  import/export r2 project in zip form (.zrp extension)
```

### Project Import and Export

Radare2 allows you to import and export projects as zip files using the `Pz` command. This can be helpful for sharing projects across different systems or for backup purposes.

To export the currently loaded project into a zip file:

```console
[0x00000000]> Pze foo.zrp
```

The `.zrp` file (Zipped Radare Project) can later be imported using `Pzi`. This will load the contents into your project home directory and commit a new version if versioning is enabled.

```console
[0x00000000]> Pzi foo.zrp
```

### Saving a Project

When you save a project, Radare2 generates an `.r2` script in the project's directory. This script includes all the commands necessary to recreate the current session. To save a project manually, use the `Ps` command:

```console
[0x00000000]> Ps my_project
[master (root-commit) b4b06ef] default message
 Author: user <user@localhost>
 1 file changed, 10549 insertions(+)
 create mode 100644 rc.r2
```

You can also use `P+` to save without checking if there was any change made since it was loaded:

```console
[0x00000000]> P+ my_project
```

### Opening and Reopening Projects

To open a project, simply use the `P` command followed by the project name. There is no need to specify the file associated with the project:

```console
[0x00000000]> P my_project
```

If you want to open the project when starting Radare2, you can use the `-p` option:

```console
$ r2 -p my_project
```

This allows you to reopen the project without specifying the binary file associated with it.

### Project Configuration Options

There are several configuration options available to control how projects are stored and managed. You can view these options using the following command:

```console
[0x00000000]> e prj.
```

Here are some key project-related options:

* `prj.alwaysprompt`: Determines whether Radare2 always prompts before saving a project.
* `prj.files`: Specifies whether to save a copy of the analyzed binary in the project directory.
* `prj.gpg`: Enables encryption of project files using GPG.
* `prj.history`: Specifies whether to save the command history with the project.
* `prj.name`: Stores the name of the current project.
* `prj.sandbox`: Isolates the project in a sandboxed environment.
* `prj.vc`: Enables version control for projects.
* `prj.vc.message`: Stores a commit message for the project (if version control is enabled).
* `prj.vc.type`: Specifies the version control system used (e.g., Git).
* `prj.zip`: Determines whether to save the project in a zip format.

These options allow you to customize project behavior based on your needs.

### Projects in Iaito

Projects also work in Iaito, the graphical interface for Radare2. In Iaito, project management options are available through the user interface, making it easy to create, open, and save projects without using the command line.

When starting **iaito** a dialog to select a file or project is shown, those projects reuse the same commands and apis than radare2, both tools may behave the same way.
