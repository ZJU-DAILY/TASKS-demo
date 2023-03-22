chs = [chr(i) for i in range(98, 123)]
for i in range(0, 1):
    file = open(""+chs[i]+"_empty.svg")
    text = file.readline()
    file.close()
    text2 = text.replace("707070", "2c2c2c").replace("707070", "FFFFFF")
    print(text2)
    # fileW = open("./fill/"+chs[i]+"_fill.svg", "w")

    # fileW.write(text2)
