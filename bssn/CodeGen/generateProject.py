import sys
import os.path
import nlsmParams as paramDef
from shutil import copyfile
from cogapp import Cog
from projectPaths import inputPath, outputPath, dendroRootPath, dendroLibPath

#create a temporary copy of the param file imported above
#cog sections all import the temp file, so only need to change the import in 1 place (here)
paramFile = os.path.abspath(paramDef.__file__)
copyfile(paramFile, inputPath + "paramDef.py")

#these files will be autogenerated entirely (without cog)
paramDef.parameters.writeJson(outputPath + "par/nlsm.par.json")
paramDef.parameters.writeCpp(outputPath + "include/parameters.h", outputPath + "src/parameters.cpp", paramDef.namespace)

args = ["cog","-o",outputPath + "CMakeLists.txt", inputPath + "CMakeLists.txt.in"]
ret = Cog().main(args)

srcFiles = ["derivs.cpp.in",
			"nlsm.cpp.in",
			"nlsm_eqs.cpp.in",
			"nlsmUtils.cpp.in",
			"profile_params.cpp.in",
			"rhs.cpp.in",
			"rk4nlsm.cpp.in"]
for file in srcFiles:
	args = ["cog","-o",outputPath + "src/" + file[:len(file)-3], inputPath + "src/" + file]
	ret = Cog().main(args)

includeFiles = ["derivs.h.in",
			"nlsm.h.in",
			"nlsmUtils.h.in",
			"profile_params.h.in",
			"rhs.h.in",
			"rk4nlsm.h.in"]

for file in includeFiles:
	args = ["cog","-o",outputPath + "include/" + file[:len(file)-3], inputPath + "include/" + file]
	ret = Cog().main(args)

os.remove(inputPath + "paramDef.py")