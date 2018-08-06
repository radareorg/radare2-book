#!/bin/sh
git log | grep ^Author |cut -d : -f2- | cut -d '<' -f 1 | sort -u | awk 'BEGIN{N=0}{
gsub(/\n/,"",$0); if (N>0) {print ","$0; } else {print $0;} ;N++} END{print "."}'
