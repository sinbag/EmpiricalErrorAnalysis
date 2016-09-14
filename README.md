# EmpiricalErrorAnalysis

Soure code for the ACM SIGGRAPH Courses 2016 on:

Fourier analysis of numerical integration in Monte Carlo rendering: Theory and Practice, authored by Kartic Subr, Gurprit Singh, Wojciech Jarosz. Avaiable on [ACM](http://dl.acm.org/citation.cfm?id=2927356)

* Source code includes Fourier/variance/mse analyzers for different sampling patterns.
* Dependencies:
 * CGAL (used for PWConstant and QuadPixel integrands in the interface)
 * OpenMP (can be made optional)
 * Python (only required for PBRTIntegrand, the source code still compiles and run for other integrands without Python)
* Use recursive tag to clone from git to add the Openexr submodule:
```
git clone —-recursive url-to-the-repo.git
````
* If you accidentally already cloned FAS2016 without this flag, run the following command to also fetch the OpenEXR dependency:
````
git submodule update --init --recursive
````
* The interface supports different types of integrand. You can hit:
````
./build/eea or ./build/eea -h
````
to see the command line usage. Example to call Disk Integrand for variance analysis is as follows:
````
./build/exec -S --stype stratified -I Disk --rad 0.25 --center 0.5 0.5 -A --atype var --nsamps  9 36 100 512  --nreps 1000 -G --ofile testDisk
````
* The interface also allow user to call PBRT-v3 code directly using a python script:
 * To perform Variance analysis for pbrt-v3 generated images, user can directly call pbrt from the provided Analysis code (look for PBRTIntegrand in the code). Make sure you have a cropwindow defined in the .pbrt scene file to select the region you are interested in.
Example to call PBRTIntegrand (all in one line):
````
./build/eea -S --stype stratified 
-I --itype Pbrt 
--epath path-to-pbrt-v3-executable/pbrt 
--spath path-to-pbrt-v3-scenes/anim-killeroos.pbrt 
--pypath path-to-pythonScript/FAS2016/code/Analysis/python/pbrt-cl.py 
--crop 0.25 0.75 0.25 0.75 
--img pbrt-eea.exr 
-A --atype var --nsamps 9 16 25 36 64 --nreps 200 
-G --ofile pbrt-killeroos
````
