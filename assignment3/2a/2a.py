#!/usr/bin/env python
# coding: utf-8

# In[2]:


import meshio

mesh = meshio.read(
    "problem2Data(1).vtk",
)

print("data loaded")


# In[3]:


import numpy as np


cellType = "tetra"

pd1 = "MagDerivatives"
pd2 = "ttkOffsetScalarField"

PointsCount = len(mesh.points)
CellsCount = len(mesh.cells_dict[cellType])

print(PointsCount)
                                    
neighbors = {}

    
for cell in mesh.cells_dict[cellType]:
    for p in cell:
        if p not in neighbors:
            neighbors[p] = set()
        for q in cell:
            if p != q:
                neighbors[p].add(q)
                #neighbors[q].add(p)
                
print("pointToCell")


# In[4]:


scalar = {}
for p in range(PointsCount):
    scalar[p] =   mesh.point_data[pd2][p] + mesh.point_data[pd1][p][0]
    
print(len(scalar))


# In[4]:


'''p = 2
print(neighbors[p])
greater = set()

for x in neighbors[p]:
    if scalar[x] > scalar[p]:
        greater.add(x)


print(greater)

connectedG = set()
elem = greater.pop()
q = [elem]

while len(q) != 0 and len(greater) != 0:
    elem = q.pop(0)
    
    connectedG.add(elem)

    for x in greater:
        if x in neighbors[elem]:
            q.append(x)
            
    for x in q:
        if x in greater:
            greater.remove(x)

    print(elem, q, greater)
            
print(greater, connectedG)

'''


# In[5]:


'''p = 2
print(neighbors[p])
greater = set()

for x in neighbors[p]:
    if scalar[x] > scalar[p]:
        greater.add(x)

print(greater)

connectedG = set()
elem = greater.pop()
q = [elem]

while len(q) != 0 and len(greater) != 0:
    elem = q.pop(0)
    
    connectedG.add(elem)
    #print(neighbors[elem])

    s = set()
    for x in greater:
        ss = neighbors[elem].intersection(neighbors[x])
        for y in ss:
            if(scalar[y] > scalar[p]):
                s.add(x)
    
    s = s.union( neighbors[elem].intersection(greater) )
    for x in s:
        q.append(x)
        greater.remove(x)
    

    print(elem, q, greater)
            
print(greater)


'''


# In[49]:


'''p = 886137

print(neighbors[p])
greater = set()

for x in neighbors[p]:
    if scalar[x] < scalar[p]:
        greater.add(x)

print(greater)

elem= greater.pop()
q = [elem]
cnt = 300000

visited = {}

while len(q) != 0 and len(greater) != 0 and cnt != 0:
    elem = q.pop(0)
    
    if elem in visited:
        continue
    
    visited[elem] = 1
    cnt -= 1
    
    for y in neighbors[elem]:
        if y in greater:
            q.append(y)
            greater.remove(y)
    
    for y in neighbors[elem]:
        if scalar[y] < scalar[p]:
            q.append(y)
            
    #print(cnt,q,  greater)
            
print(greater)

'''


# In[5]:


count = 0
maximas = []
minimas = []
possibleSaddle = []

for point in range(PointsCount):
     
    greater = set()
    lesser = set()
    
    
    for n in neighbors[point]:
        if(scalar[n] > scalar[point]):
            greater.add(n)
        elif (scalar[n] < scalar[point]): 
            lesser.add(n)
            
    
    if len(greater) == 0:
        count += 1
        maximas.append(point)
    
    elif len(lesser) == 0:
        count += 1
        minimas.append(point)
    
    else:
        if(len(neighbors[point]) < 18): 
            continue
        
        # bfs
        elem = greater.pop()
        q = [elem]

        while len(q) != 0 and len(greater) != 0:
            elem = q.pop(0)

            for x in greater:
                if x in neighbors[elem]:
                    q.append(x)
            
            for x in q:
                if x in greater:
                    greater.remove(x)
                    
        
        elem = lesser.pop()
        q = [elem]

        while len(q) != 0 and len(lesser) != 0:
            elem = q.pop(0)

            for x in lesser:
                if x in neighbors[elem]:
                    q.append(x)
            
            for x in q:
                if x in lesser:
                    lesser.remove(x)

        if len(greater) != 0 or len(lesser) != 0:
            count += 1         
            possibleSaddle.append(point)
        

print(count)


# In[10]:


print(minimas)
print(maximas)

for i in range(len(possibleSaddle)):
    if possibleSaddle[i] == 39488:
        print(i)
        
for x in minimas:
    print(x, len(neighbors[x]), mesh.point_data[pd1][x][0] )
        
for x in maximas:
    print(x, len(neighbors[x]), mesh.point_data[pd1][x][0] )
        


# In[9]:


count = 0
 
possibleSaddle2 = []

for point in possibleSaddle:
    
    #if(len(neighbors[point]) < 18): 
    #    continue
    
     
    greater = set()
    lesser = set()

    for n in neighbors[point]:
        if(scalar[n] > scalar[point]):
            greater.add(n)
        if(scalar[n] < scalar[point]):
            lesser.add(n)

    
    elem = greater.pop()
    q = [elem]
    cnt = 1000
    visited = {}

    while len(q) != 0 and len(greater) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in greater:
                q.append(y)
                greater.remove(y)

        for y in neighbors[elem]:
            if scalar[y] > scalar[point]:
                q.append(y)

    if len(greater) != 0:
        count += 1         
        possibleSaddle2.append(point)
        continue
        
    elem = lesser.pop()
    q = [elem]
    cnt = 1000
    visited = {}

    while len(q) != 0 and len(lesser) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in lesser:
                q.append(y)
                lesser.remove(y)

        for y in neighbors[elem]:
            if scalar[y] < scalar[point]:
                q.append(y)
           
                    
    if len(lesser) != 0:
        count += 1         
        possibleSaddle2.append(point)
        
        '''print(point)
        if(count > 100):
            break'''

        
print(count, len(possibleSaddle2))

# 1558802
# 124952


# In[10]:


count = 0
 
possibleSaddle2 = []

for point in possibleSaddle:
    
    #if(len(neighbors[point]) < 18): 
    #    continue
    
     
    greater = set()
    lesser = set()

    for n in neighbors[point]:
        if(scalar[n] > scalar[point]):
            greater.add(n)
        if(scalar[n] < scalar[point]):
            lesser.add(n)

    
    elem = greater.pop()
    q = [elem]
    cnt = 10000
    visited = {}

    while len(q) != 0 and len(greater) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in greater:
                q.append(y)
                greater.remove(y)

        for y in neighbors[elem]:
            if scalar[y] > scalar[point]:
                q.append(y)

    if len(greater) != 0:
        count += 1         
        possibleSaddle2.append(point)
        continue
        
    elem = lesser.pop()
    q = [elem]
    cnt = 10000
    visited = {}

    while len(q) != 0 and len(lesser) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in lesser:
                q.append(y)
                lesser.remove(y)

        for y in neighbors[elem]:
            if scalar[y] < scalar[point]:
                q.append(y)
           
                    
    if len(lesser) != 0:
        count += 1         
        possibleSaddle2.append(point)
        
        '''print(point)
        if(count > 100):
            break'''

        
print(count, len(possibleSaddle2))

# 1558802
# 124952


# In[11]:


print(mesh.point_data[pd1][124226])


# In[12]:


print(mesh.point_data[pd1][1477361])
print(mesh.point_data[pd1][513])


# In[13]:


a = []
for x in neighbors[905517]:
    a.append(x - 905517)

a = sorted(a)
print(a)


# In[24]:


#print(possibleSaddle2)
'''
f = open("possibleSaddle.txt", "a")
for x in possibleSaddle2:
    f.write(str(x)+ "\n") 
#f.write("Now the file has more content!")
f.close()
print("okkkkkk")'''


# In[25]:


count = 0
 
possibleSaddle2 = []

for point in possibleSaddle:
    
    #if(len(neighbors[point]) < 18): 
    #    continue
    
     
    greater = set()
    lesser = set()

    for n in neighbors[point]:
        if(scalar[n] > scalar[point]):
            greater.add(n)
        if(scalar[n] < scalar[point]):
            lesser.add(n)

    
    elem = greater.pop()
    q = [elem]
    cnt = 100000
    visited = {}

    while len(q) != 0 and len(greater) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in greater:
                q.append(y)
                greater.remove(y)

        for y in neighbors[elem]:
            if scalar[y] > scalar[point]:
                q.append(y)

    if len(greater) != 0:
        count += 1         
        possibleSaddle2.append(point)
        continue
        
    elem = lesser.pop()
    q = [elem]
    cnt = 100000
    visited = {}

    while len(q) != 0 and len(lesser) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in lesser:
                q.append(y)
                lesser.remove(y)

        for y in neighbors[elem]:
            if scalar[y] < scalar[point]:
                q.append(y)
           
                    
    if len(lesser) != 0:
        count += 1         
        possibleSaddle2.append(point)
        
        '''print(point)
        if(count > 100):
            break'''

        
print(count, len(possibleSaddle2))

# 1558802
# 124952


# In[27]:


#print(possibleSaddle2)

'''
f = open("possibleSaddlePart2.txt", "a")
for x in possibleSaddle2:
    f.write(str(x)+ "\n") 
#f.write("Now the file has more content!")
f.close()
print("okkkkkk")'''


# In[33]:


count = 0
 
possibleSaddle2 = []

for point in possibleSaddle:
    
    #if(len(neighbors[point]) < 18): 
    #    continue
    
     
    greater = set()
    lesser = set()

    for n in neighbors[point]:
        if(scalar[n] > scalar[point]):
            greater.add(n)
        if(scalar[n] < scalar[point]):
            lesser.add(n)

    
    elem = greater.pop()
    q = [elem]
    cnt = 200000
    visited = {}

    while len(q) != 0 and len(greater) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in greater:
                q.append(y)
                greater.remove(y)

        for y in neighbors[elem]:
            if scalar[y] > scalar[point]:
                q.append(y)

    if len(greater) != 0:
        count += 1         
        possibleSaddle2.append(point)
        continue
        
    elem = lesser.pop()
    q = [elem]
    cnt = 200000
    visited = {}

    while len(q) != 0 and len(lesser) != 0 and cnt != 0:
        elem = q.pop(0)
        
        if elem in visited:
            continue

        visited[elem] = 1
        cnt -= 1

        for y in neighbors[elem]:
            if y in lesser:
                q.append(y)
                lesser.remove(y)

        for y in neighbors[elem]:
            if scalar[y] < scalar[point]:
                q.append(y)
           
                    
    if len(lesser) != 0:
        count += 1         
        possibleSaddle2.append(point)
        
        '''print(point)
        if(count > 100):
            break'''

        
print(count, len(possibleSaddle2))

# 1558802
# 124952


# In[34]:


'''print(possibleSaddle2)


f = open("possibleSaddlePart3.txt", "a")
for x in possibleSaddle2:
    f.write(str(x)+ "\n") 
#f.write("Now the file has more content!")
f.close()
print("okkkkkk")'''


# In[39]:


for x in possibleSaddle2:
    if mesh.point_data[pd1][x][0] > 2:
        print(x, mesh.point_data[pd1][x][0])


# In[ ]:


'''
[39488, 64724, 85788, 88987, 93847, 93977, 124226, 130817, 151411, 171461, 174624, 194050, 216898, 247747, 257562, 274838, 310754, 324133, 362425, 380435, 391730, 
394210, 407308, 461285, 476301, 504950, 606468, 641698, 653711, 655157, 672529, 697882, 706594, 727587, 765131,
826167, 851393, 886137, 895450, 928749, 938230, 952084, 1028058, 1086940, 1103575, 1208426, 1239908, 1358642,
1379746, 1503719, 1511470, 1538676, 1544907]
'''


# In[1]:


print(maximas, minimas)


# In[ ]:




