
require	module, version

epicsEnvSet(MACRO, "$(MACRO=MACRO_VALUE)")

dbLoadTemplate("file.substitutions", "P=$(P=CEA:), R=$(P=LAB:)")

dbLoadRecords("file.template", "P=$(P=CEA:), R=$(P=LAB:)")