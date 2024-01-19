xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 22;
 -10.49013;-75.02677;-13.45757;,
 -10.49013;-6.06542;-13.16783;,
 10.49012;-6.06542;-13.16783;,
 10.49012;-75.02677;-13.45757;,
 -10.49013;-6.06542;-13.16783;,
 -10.49013;-6.16820;11.29444;,
 10.49012;-6.16820;11.29444;,
 10.49012;-6.06542;-13.16783;,
 -10.49013;-6.16820;11.29444;,
 -10.49013;-63.88574;2.30181;,
 10.49012;-63.88574;2.30181;,
 10.49012;-6.16820;11.29444;,
 -10.49013;-75.02677;-13.45757;,
 10.49012;-75.02677;-13.45757;,
 10.49012;-75.02677;-13.45757;,
 10.49012;-6.06542;-13.16783;,
 10.49012;-6.16820;11.29444;,
 10.49012;-63.88574;2.30181;,
 -10.49013;-63.88574;2.30181;,
 -10.49013;-6.16820;11.29444;,
 -10.49013;-6.06542;-13.16783;,
 -10.49013;-75.02677;-13.45757;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;9,12,13,10;,
 4;14,15,16,17;,
 4;18,19,20,21;;
 
 MeshMaterialList {
  1;
  6;
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\legTank.png";
   }
  }
 }
 MeshNormals {
  7;
  0.000000;0.004201;-0.999991;,
  0.000000;0.999991;0.004202;,
  0.000000;-0.526937;0.849904;,
  0.000000;-0.153947;0.988079;,
  0.000000;-0.816558;0.577263;,
  1.000000;0.000000;-0.000000;,
  -1.000000;0.000000;0.000000;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;3,2,2,3;,
  4;2,4,4,2;,
  4;5,5,5,5;,
  4;6,6,6,6;;
 }
 MeshTextureCoords {
  22;
  0.401334;0.982201;,
  0.401334;0.722942;,
  0.468549;0.722942;,
  0.468549;0.982201;,
  0.492343;0.719619;,
  0.492343;0.645545;,
  0.566417;0.645545;,
  0.566417;0.719619;,
  0.491661;0.731428;,
  0.491661;0.937482;,
  0.558705;0.937482;,
  0.558705;0.731428;,
  0.491661;0.977913;,
  0.558705;0.977913;,
  0.580983;0.986382;,
  0.578709;0.765631;,
  0.668930;0.764931;,
  0.638563;0.950058;,
  0.638563;0.950058;,
  0.668930;0.764931;,
  0.578709;0.765631;,
  0.580983;0.986382;;
 }
}
