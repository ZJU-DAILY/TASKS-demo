chs = [chr(i) for i in range(97, 123)]
for i in range(0, len(chs)):
    file = open(""+chs[i]+"_empty.svg")
    text = file.readline()
    file.close()
    # print(text.replace("707070", "2c2c2c"))
    fileW = open("./"+chs[i]+"_empty.svg", "w")
    fileW.write(text.replace("FFFFFF", "F5F5F5"))
