# file: test.cmd
# author: victor.nadot@cea.fr
# company: CEA / DRF / IRFU / LDISC
# description: this file is doing that ...
# macros:
# - >> Mandatory
# - P: ${P} | PV prefix
# - >> Optional
# - EGU: ${EGU=Volts} | PV unit

require	module, version

epicsEnvSet(MACRO, "$(MACRO=MACRO_VALUE)")

dbLoadTemplate("file.substitutions", "P=$(P=CEA:), R=$(P=LAB:)")

dbLoadRecords("file.template", "P=$(P=CEA:), R=$(P=LAB:)")