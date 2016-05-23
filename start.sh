#!/bin/sh

PS3="Choose a rom: "
cd roms
select rom in *; do
	../c8 $rom
	break
done
