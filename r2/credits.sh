#!/bin/sh
echo `git log | grep ^Author |cut -d : -f2- | cut -d '<' -f 1 |sort -u` |sed -e 's/ /, /g'
