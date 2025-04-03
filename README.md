# Project LEPL1110

Our project is divided in 3 components:
- Preprocessor
- Core solver
- Visualizer

The first and the last components are optional.

## How to create the mesh
Our mesh is already created but this is the process to generate it:

```shell
cd preprocessor
mkdir build
cd build
cmake ..
make
cd ..
./build/myFem
```

This will create `anchor.txt`.

You then need to fix the mesh using `python fixmesh.py`.
This will create `anchor_fix.txt`, you can then copy it into the data directory at the root of the project as `mesh.txt`.

## How to run the core

First make sure to have the mesh in data/mesh.txt

```shell
mkdir build
cd build
cmake ..
make
cd ..
./build/ProjetFEM
```

Make sure to run `ProjetFEM` from the root of the project because it needs to access the data folder.

## How to run the visualizer

Make sure to run the core component before to generate `data/UV.txt`

### Plot the result

```shell
python script/plot.py
```

### Plot the animated result interpolated from 0 to the final deformation

```shell
python script/plot_animated.py
```

### Plot the base mesh

```shell
python script/plot_mesh.py
```

