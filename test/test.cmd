

epicsEnvSet("MACRO", "MACRO_VALUE") 


epicsEnvSet(ACQ_IFC1410,    "ICS tag 345")
epicsEnvSet(AREA,           "$(AREA=CEA)")  # default prefix is "CEA"
epicsEnvSet(DEVICE,         "PBI-nBLM")
epicsEnvSet(HV_LV_PREFIX,   "SY4527")       # service name of HV/LV


#### hardware connection layer

### CAEN power supply (HV + LV)
epicsEnvSet(LV_SLOT,        "10")           # only one LV board for CEA teststand
epicsEnvSet(HV_SLOT,        "02")           # only one HV board for CEA teststand
dbLoadRecords("CAEN_nblm.db", "AREA=${AREA}, DEVICE=$(DEVICE), HV_LV_PREFIX=${HV_LV_PREFIX}, LV_SLOT=${LV_SLOT}, HV_SLOT=${HV_SLOT}")

"${AREA}"
"$(AREA)"

dbLoadRecords("CAEN_nblm.db", "AREA=${AREA=CEA}")
dbLoadRecords("CAEN_nblm.db", "AREA=${AREA}")
