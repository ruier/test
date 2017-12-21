@REM Run the patch405b utility to avoid 405GP rev B errata
patch405b bootrom_uncmp
@REM Run the rest of the bootrom build (from the vxworks makefile)
romsize PPC -b 0007f000 bootrom_uncmp
elfHex -a 100 bootrom_uncmp > bootrom_uncmp.hex


