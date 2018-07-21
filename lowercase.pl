#!/usr/bin/perl
#
# Lowercase a file.
# Much faster on big files than sed or tr.
#
# Some tools depends on lowercase hex when
# comparing if hashes are identical.
# Case insensitive comparing is much slower and
# these files are sometimes literally huge.
use strict;
use warnings;

my $filename = $ARGV[0];
if (not defined $filename) { die "Usage: lowercase.pl <filename>" }

open my $fh, $filename or die "Could not open $filename: $!";

while(my $line = <$fh>) {
	print lc $line;
}

close($fh);
