# basic-x86-kernel
The basics of a x86 kernel

    /*********************************/

    Compile on Linux, just tested under full updated Mint
        First install 
            sudo apt install build-essential     # The essentials build tools
            sudo apt install gcc-multilib        # To allow compile 32bits elf files
            sudo apt install  cmake              # The cmake for CMakeLists.txt

        Then you are ready to go compile...

        Build:
            cd build
            rm -fr *
            cmake ../
            make

    /*********************************/

    Compile on Windows, under msys using i686-elf-tools
        
        First require msys with cmake package ..
            Get it at: https://www.msys2.org
        
        Second require the cross compiler ...
            Get it at: https://github.com/lordmilko/i686-elf-tools

        Third ajusts the:
            CMakeLists.txt inside the klib and ksrc
            to point to the cross compiler elf tools

        Fourth Build:
            cd build
            del /f /s /q *
            cmake ../
            make

Have a nice day ;), and leave a Star *



