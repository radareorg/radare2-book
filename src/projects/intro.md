# Projects

There are some scenarios where you need to work for a long period of time on a single or a set of binaries.

Sometimes when working with large binaries or even if it's small you want to store and keep your progress, the comments, function names, and other metadata so you don't have to handle it when loading the binary again.

This chapter will cover all these needs by exposing the challenges and limitations because, despite looking like a simple problem, projects is one of the hardest issues to cope, and first we need to understand why.

## Challenges

Metadata associated with a binary analysis is an important feature to support for all reverse engineering tools for several reasons, let's explore some of them:

* There's no standard format for saving or sharing it.
* Tools change over time, its analysis and metadata too.
* Metadata storage order matters, you can't name a function if its not analyzed
* Analysis order and steps can affect the final result
* Projects can be versioned, rebasing it can result on unexpected results
* Syncing metadata in realtime between different clients can cause conflicts
* Amount of data tends to be large, storing/loading is slower than keeping it in memory
* Binaries can be loaded in different addresses, aslr when debugging
* User settings affect analysis and metadata registration
* Incremental metadata patches must be stacked up properly

After checking this list we observe how difficult the problem is, and how many of the solutions don't fit in all the possible environments and use cases users will face.

In the case of **radare2**, as long as the tool permits creating so many different configuration paths it is harder to find a way to serialize project information into a file and restoring it back compared to other tools which are tied to much less options.
