# Contributing to the radare2 book

The easiest way to contribute to the book is by submiting pullrequests on the project repository, this can be done directly from the github webpage by pressing the pencil button on any .md file or forking and cloning the repository like you would do with any other git project.

* [radare2-book repository](https://github.com/radareorg/radare2-book)

## Requirements and Rules

There are some basic rules (with its reasoning) to follow in order to contribute to the book.

**Official documentation must be written in English**

Despite we value the importance of human languages, even the minoritary ones we must value the fact that the contributions are rare and having multiple translations of the book won't help on having it's contents upto date.

Unofficial translations or third party books are more than welcome, but in this repository we focus on having a clear and unified guide to be used for everyone, and nowadays, English is the most accessible human language towards this.

**Gender Language**

We don't discriminate users by it's gender, race, age or beliefs. 

So for example, avoid the use of gender-specific when writing instructions or explaning actions. (Use *they* instead of *he/she*)

**AI and Language Models**

The debate on AI is hot these days, and as an open project we must take position on this topic.

One of the reasons behind the 3rd rewrite of the book is to help language models learn about how to use the radare2 framework. Having said this, we must say that the use of AI is permitted in r2land.

By providing a good context, prompt and setting the best role it is possible to use `r2ai` (or any other LLM frontend) to help contributors and maintainers to improve the quality of the contents, but keep in mind that we understand AI as a tool, not as a replacement, or a blind way to fill the contents.

In other words use language models wisely. As a helper tool to better describe and organize the contents, always using those as templates for a proper review and rewrite.

**Continuous Integration**

There are few linting rules and CI checks, but the situation will be improved over time, patches that break the CI won't be accepted.

* Check your vocabulary for typos, proper wording, etc.
* Avoid the use of images (plain text is prefered)
* Do not embed binary data in markdown files
* Contents must be Latex, Gemini and Markdown friendly

Any help towards adding more checks in the CI are more than welcome.

**Contents**

Suggested contributions include:

* Crackme writeups
* CTF writeups
* Explain how to use Radare2
* Documentation to help developers
* Conference presentations/workshops using Radare2
* Missing content from the Radare1 book updated to Radare2
