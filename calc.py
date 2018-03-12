import re

classes = dict()
def register(cls_,name):
    classes[name] = cls_

# deal with lab1's data
lab1_raw = open('data_extended2.txt')
for line in lab1_raw:
    if(re.search('-----([^-]+)-----scale:\s*(\d+)\s.*Execution time:\s*(\d+)',line)):
        result = re.match('-----([^-]+)-----scale:\s*(\d+)\s.*Execution time:\s*(\d+)',line)
        if result.group(1).replace(' ','') not in classes.keys():
            # create new map if not created
            a= dict()
            a[int(result.group(2))] = int(result.group(3))
            register(a,result.group(1).replace(' ',''))
        else:
            set = classes[result.group(1).replace(' ','')]
            set[int(result.group(2))] = int(result.group(3))

# output
with open('data_extend_sorted2.txt','w') as f:
    f.write('{')
    for class_ in classes:
        f.write('{')
        for key,value in classes[class_].items():
            f.write('{'+str(key)+','+str(value)+'},')
        f.seek(f.tell()-1,0)
        f.write('},')
    f.seek(f.tell()-1,0)
    f.write('}')
        
pass