# wavecrush

__wavecrush__ operates on WAV files. It takes in a file, runs the algorithms which user choses
on the "data" chunk of the file and writes the modified file to the folder which the program
is located.
  
  - Think of an audio file as a collection of samples
  - Create new musical ideas and creating new samples
  - Perfect for glitch
  - wavecrush creates unexpected transients which play well with filters for further processing

# Not ready yet!

  - Though the app works ok it is not mature yet but feel free to try or contribute,
  detailed comments in code and detailed readme will come soon.
  - There is a concise documentation printed when you `--help`

# Build
  
  - Builds ok with `g++ version 9.0.1` which i have used in Mac OS X Sierra.
  - I use the latest version of gcc, and i installed it with homebrew. `brew install gcc --HEAD`
  ```sh
  g++ -Wall -std=c++1z ./*.cpp -o wavecrush
  ```

### Todos

- Properly comment code
- Write a clear readme and explanations
 - Link audio samples 
 - Create a make file
 - Integrate Trvais CI and write tests

