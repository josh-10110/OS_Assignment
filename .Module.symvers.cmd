cmd_/media/josh/ESD-USB/test2/Module.symvers := sed 's/\.ko$$/\.o/' /media/josh/ESD-USB/test2/modules.order | scripts/mod/modpost -m -a  -o /media/josh/ESD-USB/test2/Module.symvers -e -i Module.symvers   -T -