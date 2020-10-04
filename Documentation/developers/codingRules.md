Coding Rules
============

C++ is the preferred development language. However, the use of C is also allowed. Libraries used are typically C++ and C. Fortran 
libraries and code specifically for numeric type manipulation will also be allowed.

1. All new code should be written using C++.
2. The use of standard C++ and C libraries is encouraged, including the use of the STL.
3. The API used for development is the Qt API. This was specifically done to enable easy porting of the application to multiple
platforms. The Qt framework should be used as far as possible for AIRDAS specific code. This includes all functionality provided by 
the Qt framework.
4. All code being developed for ACL, GCL, MCL, PCL, SCL libraries should be independent of the Qt framework. The intent is to keep 
these libraries general and allow them to be used with any front-end code required. AstroManager handles the user interface, while 
the libraries do all the heavy lifting.
5. Any database server supported by Qt can be used. Where the database allows embedded code (such as PL/SQL in Oracle), the 
developer can take a view on whether to make use of the embedded code functionality. However, if code is embedded into a database, 
the developer must make the embedded code available with the software, or provide the functionality in the application code to 
bypass the need for the embedded code. The database is viewed to be part of the front-end code.
6. The code should be written to compile with any of the popular compilers. Where language differences exist between compilers, then 
the differences should be resolved using #ifdef statements that refer to the compiler.
7. All counters and indexes of arrays should be based on size_t. This is required for 64bit compilation.
8. All pointers should be based on ptrdiff_t. This is required for 64bit compilation.
9. Compile time type-casting must be done using static_cast<>() and not the traditional C (type) cast.
10. Run time type-casting must be done using dynamic_cast<>(). Checks for non nullptr return values should also be done. Throw 
CODE_ERROR if nullptr values are returned.
11. Use the specific C-types to give standard integer variable sizes.
    Use std::uint8_t, std::uint16_t etc to ensure that variables have correct sizes for the application. This will also make it 
      visible to the developer the range of the variable.
    Use int where the variable size does not matter, or to interface with existing libraries. 
12. No naked pointers. All object must be owned by a smart  pointer. When ownership is not being transferred and pointers may
    be passed to functions. 
13. Factory functions should return std::unique<>() pointers.
14. Functions having pointer parameters when ownership is not to be transferred use C-style pointers.
15. Never delete an object. The smart_ptr or unique_ptr will delete it when it goes out of scope.
