#the files in CogInputTestBuild are direct copies from Dendro/NLSigma. No cog involved.
#The point is to just use them to easily test the cogged CMakeList.txt (since the files themselves are all valid and will build)
#actual cogged src files are in CogInput folder, but until they're all finished the results won't build.
inputPath = "./SymPyGR/bssn/CodeGen/CogInputTestBuild/"

#TODO: get this from argv once other stuff is more polished
outputPath = "./output/" 

#should be relative to the outputPath
dendroPath = "../Dendro-5.01"