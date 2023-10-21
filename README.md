# CS3520 - PA02 - Image Processing in C++

Developer Name: _Harrison Eckert_

## Notes to TAs

In order to use the `write` command or any transformation commands, an image must first be successfully read with `read`

Some sample images are pre-included in `/sampleImages/`, including my adorable cats (`taffy.jpeg` and `baymax.jpeg`). Have fun!

## How to Run the Program

This program runs a controller via `main()`. After compiling, run:

`./cmake-build-debug/ImageLib`

This will begin the controller loop, where a user can enter a number of commands. The commands supported are:
- `read` - reads from a file
  - ***In order to `write` or perform a transformation, `read` must first be successfully executed***
  - All files are assumed to be in `/sampleImages/`
  - This command will prompt the user for relevant information: 
    - filename 
    - width
    - height
    - what components the image was encoded with (default RGB)
    - and what components the **output** image should be encoded with (default RGB)
  

- `write` - writes to a file
  - All files will be written to `/sampleImages/`
  - All writes will be performed upon program termination
  - This command will prompt the user for relevant information:
    - name of the output file
    - and the output image format (default PNG)
  

- `vflip` - flips an image on the vertical axis
- `hflip` - flips an image on the horizontal axis
- `+90` - rotates an image by 90 degrees
- `-90` - rotates an image by -90 degrees

- `border` - adds a border to an image
  - A border is implicitly RGBa. To make a grayscale border, input the same number for all components
  - This command will prompt the user for relevant information:
    - border components (R, G, B, A)
    - border width
- `pointillism` - applies a Pointillism filter to an image
- `help` - displays all commands with a short explanation
- `exit` - exit the controller, and exports all written images

