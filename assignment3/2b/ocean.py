#!/usr/bin/env python
# coding: utf-8

# In[2]:


import meshio
import numpy as np

mesh = meshio.read(
    "ass3_ocean.vtk"
)



PointsCount = len(mesh.points)
CellsCount = len(mesh.cells_dict['quad'])       

#onLand = np.zeros(PointsCount, float)        

print(PointsCount)


# In[3]:


count = 0
for i in range(PointsCount):
    if mesh.point_data['zos'][i] < 0:
        count += 1
        
print(count)


# In[4]:


print(mesh.points)
print(mesh.cells_dict['quad'])   


neighbors = {}

for cell in mesh.cells_dict['quad']:
    for point in cell:
        if point not in neighbors:
            neighbors[point] = set()
            
        for otherP in cell:
            if otherP != point:
                neighbors[point].add(otherP)
                
print(neighbors[254])


# In[5]:


p = 49264
print(mesh.points[p])
print(neighbors[p])

for n in neighbors[p]:
    print(mesh.point_data['zos'][n])


# In[23]:


count = 0

mp = {}

for p in range(PointsCount):
    if mesh.point_data['zos'][p] < 0: # or len(neighbors[p]) < 8:
        continue
    
    flag = False
    for n in neighbors[p]:
        if mesh.point_data['zos'][p] < mesh.point_data['zos'][n]: # or mesh.point_data['zos'][n] < 0:
            flag = True
            break
    
    if flag == False:
        #print(p)
        count += 1
        mp[p] = 1

print(count, len(mp))


# In[24]:


'''f = open("oceanEqual.txt", "a")

for p in range(PointsCount):
    x = mesh.point_data['zos'][p]
    if p in mp:
        x = 2
    
    f.write(str(x) + "\n")

f.close()
print("okkkkkk")'''


# In[7]:


count = 0

mp = {}

for p in range(PointsCount):
    if mesh.point_data['zos'][p] < 0: # or len(neighbors[p]) < 8:
        continue
    
    flag = False
    for n in neighbors[p]:
        if mesh.point_data['zos'][p] <= mesh.point_data['zos'][n]: # or mesh.point_data['zos'][n] < 0:
            flag = True
            break
    
    if flag == False:
        #print(p)
        count += 1
        mp[p] = 1

print(count, len(mp))


# In[19]:


'''f = open("ocean.txt", "a")

for p in range(PointsCount):
    x = mesh.point_data['zos'][p]
    if p in mp:
        x = 2
    
    f.write(str(x) + "\n")

f.close()
print("okkkkkk")'''


# In[29]:


# without border
count = 0

mp = {}

for p in range(PointsCount):
    if mesh.point_data['zos'][p] < 0 or len(neighbors[p]) < 8:
        continue
    
    flag = False
    for n in neighbors[p]:
        if mesh.point_data['zos'][p] <= mesh.point_data['zos'][n] or mesh.point_data['zos'][n] < 0:
            flag = True
            break
    
    if flag == False:
        print(mesh.points[p])
        count += 1
        mp[p] = 1

print(count, len(mp))


# In[30]:


'''print(len(mp))
f = open("oceanStrictlyGreater.vtk", "a")

for p in range(PointsCount):
    x = mesh.point_data['zos'][p]
    if p in mp:
        x = 2
    
    f.write(str(x) + "\n")

f.close()
print("okkkkkk")'''


# In[11]:


'''count = 0


newScalar = []
pointCoord = []
pointVal = []

for p in range(PointsCount):
    if mesh.point_data['zos'][p] < 0:
        newScalar.append(0) 
        continue
    
    flag = False
    for n in neighbors[p]:
        if mesh.point_data['zos'][p] < mesh.point_data['zos'][n] or mesh.point_data['zos'][n] < 0:
            flag = True
            break
    
    if flag == False:
        #print(p)
        newScalar.append(1) 
        pointCoord.append(list(mesh.points[p]))
        pointVal.append(1)
        count += 1
    else:
        newScalar.append(0) 
        
print(count)'''


# In[12]:


'''print(len(newScalar))
print(pointVal)
#print(newScalar)

#print(pointCoord)
'''


# In[13]:


'''mesh2 = meshio.Mesh(
    pointCoord, 
    cells, 
    point_data={"T": pointVal}

)
mesh2.write(
    "foo.vtk",  # str, os.PathLike, or buffer/open file
    file_format="vtk",  # optional if first argument is a path; inferred from extension
)

print("ookkkk")'''


# In[14]:


'''# two triangles and one quad
points = [
    [0.0, 0.0, 0.0],
    [1.0, 0.0, 0.0],
    [0.0, 1.0, 0.0],
    
]
cells = [
       # ("vertex", [[0], [1], [2]]),
]

mesh2 = meshio.Mesh(
    points,
    cells,
    # Optionally provide extra data on points, cells, etc.
    #point_data={"T": [0.3, -1.2, 0.5, 0.7, 0.0, -3.0]},
    # Each item in cell data must match the cells array
    #cell_data={"a": [[0.1, 0.2], [0.4]]},
)
mesh2.write(
    "fff.vtk",  # str, os.PathLike, or buffer/open file
    file_format="vtk",  #
    binary = False
)
print("ookkkk")'''


# In[15]:


'''f = open("demo.txt", "a")
for x in newScalar:
    f.write(str(x)+ "\n") 
#f.write("Now the file has more content!")
f.close()
print("okkkkkk")'''


# In[ ]:




