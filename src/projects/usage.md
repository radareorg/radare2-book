## Using projects

Projects are the way metadata of your session is saved and restored. All that stuff is stored in a directory. You can open a file, start working on it and then use the `P` command to name the project.

At this moment, an .r2 script is created in the projects directory containing all the commands needed to recreate the state.

```
[0x00000000]> P?
Usage: P[?.+-*cdilnsS] [file]  Project management
| P [file]          open project (formerly Po)
| P.                show current loaded project (see prj.name)
| P+ [name]         save project (same as Ps, but doesnt checks for changes)
| P- [name]         delete project
| P*                printn project script as r2 commands
| P!([cmd])         open a shell or run command in the project directory
| Pc                close current project
| Pd [N]            diff Nth commit
| Pi [file]         show project information
| Pl                list all projects
| Pn -              edit current loaded project notes using cfg.editor
| Pn[j]             manage notes associated with the project
| Ps [file]         save project (see dir.projects)
| PS [file]         save script file
| PS* [name]        print the project script file (Like P*, but requires a project)
| Pz[ie] [zipfile]  import/export r2 project in zip form (.zrp extension)
| NOTE:             the 'e prj.name' evar can save/open/rename/list projects.
| NOTE:             see the other 'e??prj.' evars for more options.
| NOTE:             project are stored in dir.projects
[0x00000000]>
```

When leaving, if you want to reopen the project just run it like this: `r2 -p prjname`. No need to specify the file name associated.

```
 -p [prj]     use project, list if no arg, load if no file
```

Projects work in `iaito` too and there are several options that may help you tweak the information you want to store. But also, how you want it to be stored.

```
[0x00000000]> e prj.
prj.alwasyprompt = false
prj.files = false
prj.gpg = false
prj.history = false
prj.name =
prj.sandbox = false
prj.vc = true
prj.vc.message =
prj.vc.type = git
prj.zip = false
```
