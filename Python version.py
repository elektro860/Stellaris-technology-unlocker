import os

def getFileType(file):
    file = str.split(file, ".")
    if file.__len__() == 0:
        return "Directory"
    return file[file.__len__()-1]

def getTechnologies(file):
    techFile = open(target+"/"+file, "r")
    content = techFile.read()
    techFile.close()

    technologies = []
    state = "Parsing"
    level = 0

    for i in range(1, content.__len__()-1):
        char = content[i]
        if char == "\n":
            state = "Parsing"
            continue
        if state == "Skipping":
            continue
        if char == "#":
            state = "Skipping"
        if char == "{":
            level += 1
            if level == 1:
                k = i-1
                while(content[k] != "\n"):
                    char = content[k]
                    k -= 1
                k += 1
                technologies.append("")
                index = technologies.__len__()-1
                while(content[k] != "="):
                    char = content[k]
                    k += 1
                    if char == " " or char == " ": #Space and Tab
                        continue
                    technologies[index] = technologies[index] + char    

        if char == "}":
            level -= 1

    return technologies

target = "D:/SteamLibrary/steamapps/common/Stellaris/common/technology"

if os.path.exists("out.txt"):
    os.remove("out.txt")

f = open("out.txt", "a")
f.write("Output:\n")

files = os.listdir(target)
techFiles = []
for file in files:
    if getFileType(file) == "txt":
        techFiles.append(file)


#for file in techFiles:
#    f.write("\n"+"File: "+file+"\n")
#    for tech in getTechnologies(file):
#        f.write("Technology - " + tech + "\n")
techs = 0

f.write("Unlock_technologies = {\n  ")

for file in techFiles:
    for tech in getTechnologies(file):
        techs+=1
        f.write("give_technology={tech=" + tech+"}" + " ")


f.write("\n}")

f.close()

print("There are currently " + techs.__str__() + " technologies in the game")