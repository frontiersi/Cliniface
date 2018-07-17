////////////////////////////////////////////////////////////////////////////////
//
// (C) 2012, Michail Vidiassov, John C. Bowman, Alexander Grahn
//
// asylabels.js
//
// version 20120912
//
////////////////////////////////////////////////////////////////////////////////
//
// 3D JavaScript to be used with media9.sty (option `add3Djscript') for
// Asymptote generated PRC files
//
// adds billboard behaviour to text labels in Asymptote PRC files so that
// they always face the camera under 3D rotation.
//
//
// This work may be distributed and/or modified under the
// conditions of the LaTeX Project Public License.
// 
// The latest version of this license is in
//   http://mirrors.ctan.org/macros/latex/base/lppl.txt
// 
// This work has the LPPL maintenance status `maintained'.
// 
// The Current Maintainer of this work is A. Grahn.
//
////////////////////////////////////////////////////////////////////////////////

var bbnodes=new Array(); // billboard meshes
var bbtrans=new Array(); // billboard transforms

function fulltransform(mesh) 
{ 
  var t=new Matrix4x4(mesh.transform); 
  if(mesh.parent.name != "") { 
    var parentTransform=fulltransform(mesh.parent); 
    t.multiplyInPlace(parentTransform); 
    return t; 
  } else
    return t; 
} 

// find all text labels in the scene and determine pivoting points
var nodes=scene.nodes;
var nodescount=nodes.count;
var third=1.0/3.0;
for(var i=0; i < nodescount; i++) {
  var node=nodes.getByIndex(i); 
  var name=node.name;
  var end=name.lastIndexOf(".")-1;
  if(end > 0) {
    if(name.charAt(end) == "\001") {
      var start=name.lastIndexOf("-")+1;
      if(end > start) {
        node.name=name.substr(0,start-1);
        var nodeMatrix=fulltransform(node.parent);
        var c=nodeMatrix.translation; // position
        var d=Math.pow(Math.abs(nodeMatrix.determinant),third); // scale
        bbnodes.push(node);
        bbtrans.push(Matrix4x4().scale(d,d,d).translate(c).multiply(nodeMatrix.inverse));
      }
    }
  }
}

var camera=scene.cameras.getByIndex(0); 
var zero=new Vector3(0,0,0);
var bbcount=bbnodes.length;

// event handler to maintain camera-facing text labels
billboardHandler=new RenderEventHandler();
billboardHandler.onEvent=function(event)
{
  var T=new Matrix4x4();
  T.setView(zero,camera.position.subtract(camera.targetPosition),
            camera.up.subtract(camera.position));

  for(var j=0; j < bbcount; j++)
    bbnodes[j].transform.set(T.multiply(bbtrans[j]));
  runtime.refresh(); 
}
runtime.addEventHandler(billboardHandler);

runtime.refresh();
