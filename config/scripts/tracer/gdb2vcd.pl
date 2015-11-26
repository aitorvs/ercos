#
# gdb2vcd.pl
#
# Written by Vicente Esteve LLoret <viesllo@inf.upv.es>
# Copyright (C) Feb, 2003 OCERA Consortium.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation version 2.
#
#





#!/usr/bin/perl

require 5.002;
use strict;
use FileHandle;
use Math::BigInt;
#use bignum;

###############################################################################
#                This Script get history GDB information 
#                and from x /FMT tracerbuf command get VCD file
################################################################################




################################################################################
# FUNCTION   :  use_script
# ABSTRACT   :  displays error message in case of wrong parameters
# PARAMETERS :  
# TODO       :
# COMMENTS   :  
################################################################################
sub use_script
{
  print "Use:\n";
  print "  gdb2vcd.pl history\n";
  exit;
}

################################################################################
# GLOBAL VARIABLES

# PARAMETERS: 
# object file
# inputfile: file containing functions to be studied

my $inputfile = shift || use_script;  
my $outputfile = shift || "";

my $starttime = "";

sub Insert_VCD_Header
{
  printf "\$version\n";
  printf "\$end\n";
  printf "\$timescale 1\n";
  printf "\$end\n";
  printf "\$var wire 8 0x01 0x01 \$end\n";  
  printf "\$var wire 8 0x02 0x02 \$end\n";  
  printf "\$var wire 8 0x03 0x03 \$end\n";  
  printf "\$var wire 8 0x04 0x04 \$end\n";  
  printf "\$var wire 8 0x05 0x05 \$end\n";  
  printf "\$var wire 8 0x06 0x06 \$end\n";  
  printf "\$dumpvars\n";
}

################################################################################
# FUNCTION   :  process_event
# ABSTRACT   :  Transform event format to VCD Ascii format 
# PARAMETERS :  
# TODO       :
# COMMENTS   :             
################################################################################


sub process_event
{
   my $event = $_[0];
   my $time = $event;
   my $eventid = $event;
   my $eventvalue = $event;
   
#   print $event,"\n";
   $time =~ s/(0x)(\S\S\S\S\S\S)\S\S(\S\S\S\S\S\S)\S\S/$1$2$3/;
   $time =~ s/(0x)\S\S\S\S\S\S(\S\S)\S\S\S\S\S\S\S\S/$1$2/;
   $eventid =~ s/(0x)\S\S\S\S\S\S(\S\S)\S\S\S\S\S\S\S\S/$1$2/;
   $eventvalue =~ s/(0x)\S\S\S\S\S\S\S\S\S\S\S\S\S\S(\S\S)/$1$2/;
#   printf "Time %x  Eventid %s Eventvalue %s\n",hex($time),$eventid,$eventvalue;

   if ($starttime eq "") 
   { 
     $starttime = $time;
     printf "#0\n";
   } else {
     printf "#%u\n",Math::BigInt->new($time)->bsub(Math::BigInt->new($starttime));
   };

   printf "b%08b %s\n",hex($eventvalue),$eventid;
}

################################################################################
# FUNCTION   :  read_history
# ABSTRACT   :  Read History files 
# PARAMETERS :  
# TODO       :
# COMMENTS   :             
################################################################################


sub read_history
{
  my $file = $_[0];
  my @words;

  open(RES,"cat $file | grep '<tracerbuf' |");

  Insert_VCD_Header();
  
  while (<RES>)
  {
    chomp;
    @words=split(/\s+/);
    process_event($words[2]);     
    process_event($words[3]);     
  }

  printf "\$end\n";
  close(RES);
}


################################################################################
# FUNCTION   :  main
# ABSTRACT   :  calls to other subs and generates output 
# PARAMETERS :  
# TODO       :
# COMMENTS   :             
################################################################################

read_history($inputfile);

exit;

