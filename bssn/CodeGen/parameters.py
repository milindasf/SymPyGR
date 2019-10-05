import math
from enum import Enum
import warnings

import sys    
import os
import datetime
import ntpath

from collections import OrderedDict

#holds dict of parameters, contains mostly output functions
class Parameters:
	def __init__(self, paramDict=None):
		if paramDict is None:
			paramDict = OrderedDict()
		self.paramDict = paramDict
		self.indent = "\t"

		# if set, will be used as the default category for any newly added parameters
		self.category = None
		self.generationNote = "This file was generated at {0}, through the parameters object defined in {1}".format(
								datetime.datetime.now().strftime("%I:%M%p on %B %d, %Y"),
								os.path.basename(sys.argv[0]))

	def __getitem__(self, key):
		return self.paramDict[key]

	#add is probably the simpler thing to use generally. But this gives direct access
	def __setitem__(self, key, value):
		self.paramDict[key] = value

	def add(self, id, value, description=None, category=None, cppType=None, arraySize = None):
		#set default value if it makes sense
		if category is None and self.category is not None:
			category = self.category

		if isinstance(id, PresetParam):
			param = id.value
			param.value = value
			#note the dict key will be the PresetParam enum, not a string
			self.paramDict[id] = param
		else:
			param = Parameter(id, value, description, category, cppType, arraySize)
			self.paramDict[param.id] = param

	def setCategory(self, category):
		self.category = category

	def clearCategory(self):
		self.category = None

	def values(self):
		return self.paramDict.values()

	def writeCpp(self, hPath, cppPath, namespace):

		hName = ntpath.basename(hPath)
		cppName = ntpath.basename(cppPath)

		# Initialize output and first lines for .h file
		outH = open(hPath, "w")
		outH.write("//" + self.generationNote)
		outH.write("#ifndef SFCSORTBENCH_PARAMETERS_H\n")
		outH.write("#define SFCSORTBENCH_PARAMETERS_H\n\n")

		outH.write("#include <string.h>\n")
		outH.write("#include <iostream>\n\n")

		outH.write("namespace {0}\n{{\n\n".format(namespace))

		# Initialize output and first lines for .cpp file.
		outCpp = open(cppPath, "w")
		outCpp.write("//" + self.generationNote + "\n")

		outCpp.write("#include \"{0}\"\n".format(hName))
		outCpp.write("namespace {0}\n{{\n\n".format(namespace))

		for parameter in self.paramDict.values():
			if parameter.isComment():
				continue
			outH.write(parameter.toStringH(1))
			outCpp.write(parameter.toStringCpp(1))

		outH.write("}\n")
		outH.write("#endif //SFCSORTBENCH_PARAMETERS_H")
		outH.close()

		outCpp.write("}")
		outCpp.close()

	def writeJson(self, filepath):
		categories = OrderedDict()
		for parameter in self.paramDict.values():
			if parameter.category not in categories:
				categories[parameter.category] = ""
			categories[parameter.category] += parameter.toStringJson(2)

		out = open(filepath, "w")
		out.write("{\n")
		out.write('"__comment__" : "{0}",\n'.format(self.generationNote))

		first = True
		for key in categories:
			if not first:
				out.write(",\n")
				first = False

			keyLength = len(key) if key is not None else 0
			if keyLength > 0:
				out.write('{0}"__comment__" : "{1} {2} {3}",\n'.format(self.indent, int(math.floor((50 - keyLength) / 2) - 1) * "=", key,
																	int(math.ceil((50 - keyLength) / 2) - 1) * "="))
			out.write(categories[key])
			if keyLength > 0:
				out.write('{0}"__comment__" : "{1}"\n\n'.format(self.indent, "=" * 50))

		out.write("\n}")
		out.close()

class Parameter:
	def __init__(self, id, value, description=None, category=None, cppType=None, arraySize=None):
		self.id = id
		self.value = value
		self.description = description
		self.category = category
		self.indent = "\t"
		self.arraySize = arraySize

		# if value is a list but array size isn't explicitly set, get it from values
		if self.arraySize is None and isinstance(value, list) and len(value) > 0:
			self.arraySize = len(value)
			# set temp value to first element in list, so we can get the array type below
			value = value[0]

		if isinstance(cppType, CppType):
			self.cppType = cppType
		else:
			if cppType is not None:
				warnings.warn("Invalid value for cppType arg when creating parameter. Should use CppType enum.")

			# infer data type if not set
			if isinstance(value, int):
				self.cppType = CppType.unsignedInt
			elif isinstance(value, float):
				self.cppType = CppType.double
			#default to string if nothing else matches
			else:
				self.cppType = CppType.string

	def isComment(self):
		return self.id.startswith("__comment__", 0, 11)

	def toStringJson(self, indentCount=0):
		output = ""
		if (self.description is not None):
			output += '{0}"           " : "{1}",\n'.format(self.indent * indentCount, self.description)
		output += '{0}"{1}" : {2},\n'.format(self.indent * indentCount, self.id,
											 '"'+self.value +'"' if isinstance(self.value, str) else self.value)

		return output
	def toStringH(self, indentCount = 0):
		output = ""
		if self.description is not None:
			output += "{0}//{1}\n".format(self.indent * indentCount, self.description)

		output += "{0}extern {1} {2}{3};\n\n".format("\t" * indentCount, self.cppType.value, self.id,
											  "[" + str(self.arraySize) +"]" if self.arraySize is not None else "")
		return output

	def toStringCpp(self, indentCount = 0):
		output = ""
		if self.description is not None:
			output += "{0}//{1}\n".format(self.indent * indentCount, self.description)

		# surround value with quotes if it's a string, otherwise nothing
		valueQuote = '"' if self.cppType is CppType.string else ""
		if self.arraySize is None:
			output += "{0}{1} {2}={4}{3}{4};\n\n".format(self.indent * indentCount, self.cppType.value, self.id, self.value, valueQuote)
		else:
			arrayValue = valueQuote + "{0},{0}".format(valueQuote).join(str(v) for v in self.value) + valueQuote
			output += "{0}{1} {2}[{3}]={{{4}}};\n\n".format(self.indent * indentCount, self.cppType.value, self.id, self.arraySize, arrayValue)
			
		return output

class CppType(Enum):
	unsignedInt = "unsigned int"
	double = "double"
	string = "std::string"

class PresetParam(Enum):
	#note values start out undefined, will be filled in before added to paramDict
	ASYNC_COMM_K = Parameter("ASYNC_COMM_K", None, "variable group size for the asynchronous unzip operation", cppType = CppType.unsignedInt)
