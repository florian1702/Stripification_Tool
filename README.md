<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">

<h3 align="center">Stripification Tool</h3>

  <p align="center">
    Tool which, after reading in an OBJ file, combines the triangles it contains into strips and draws them efficiently. Stripification is used for this. This method reduces the memory requirements for the vertices on the GPU, as less redundant data needs to be stored.
  </p>

</div>

<!-- ABOUT THE PROJECT -->
## About The Project
<div align="center">
  
[![Stripification YouTube Video](https://img.youtube.com/vi/nGIa2CCQX40/0.jpg)](https://youtu.be/nGIa2CCQX40)

</div>
This project focuses on optimizing the rendering of 3D models by converting individual triangles into Triangle Strips. Triangle Stripification is a technique that significantly enhances the efficiency of rendering processes by reducing redundant vertex data sent to the GPU. In a Triangle Strip, each subsequent triangle shares two vertices with the previous one, allowing a more compact representation. For instance, triangles with vertices (1, 2, 3), (3, 2, 4), and (3, 4, 5) are converted into a strip represented by the vertices (1, 2, 3, 4, 5). This reduces the need to store repeated vertices, leading to lower memory usage and faster rendering.

The main tasks achieved in this project include:

* Implementing a Triangles-to-Strip converter
* Developing a Strip drawer for efficient rendering
* Adjusting the existing code to integrate these new features
* Adding a control system and a graphical user interface (GUI)

This enhanced pipeline improves the performance and memory efficiency of rendering OBJ files, making it a key step in optimizing 3D model visualization.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With

[![C++][cpp.com]][cpp-url]

[![OpenGL][opengl.com]][opengl-url]

[![Visual Studio][vs.com]][vs-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>






<!-- CONTACT -->
## Contact

Florian Wößner - flo-woe@web.de

<p align="right">(<a href="#readme-top">back to top</a>)</p>






<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/linkedin_username
[product-screenshot]: images/screenshot.png
[cpp.com]: https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white
[cpp-url]: https://isocpp.org/
[opengl.com]: https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl
[opengl-url]: https://www.opengl.org/
[vs.com]: https://img.shields.io/badge/Visual_Studio-5C2D91?style=for-the-badge&logo=visual%20studio&logoColor=white
[vs-url]: https://visualstudio.microsoft.com/de/

