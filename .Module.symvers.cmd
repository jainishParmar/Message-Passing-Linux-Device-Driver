cmd_/home/ubuntu/ldd/Module.symvers := sed 's/\.ko$$/\.o/' /home/ubuntu/ldd/modules.order | scripts/mod/modpost -m -a  -o /home/ubuntu/ldd/Module.symvers -e -i Module.symvers   -T -
