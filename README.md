# Lucas-Kanade Optical Flow

This is a very slow, toy implementation of the Lucas-Kanade Optical Flow algorithm for self-learning purposes. Methods including 2D convolution, taking image gradients and the least squares approximation of flow velocities were done from scratch.

See example output below:

![alt-text](https://github.com/aswinvisva/optical_flow_algorithms/blob/main/example/example.png)

# Usage

1. Clone the repository
```console
git clone https://github.com/aswinvisva/optical_flow_algorithms.git
```

2. Make a build directory
```console
mkdir build
cd build
```

3. Generate the MakeFile using CMake
```console
cmake ../
```

4. Build the project
```console
make
```

5. Run with a video of your choice
```console
./OpticalFlowAlgorithms --video_dir
```
