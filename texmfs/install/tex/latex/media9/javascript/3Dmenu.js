////////////////////////////////////////////////////////////////////////////////
//
// (C) 2012--today, Alexander Grahn
//
// 3Dmenu.js
//
// version 20140923
//
////////////////////////////////////////////////////////////////////////////////
//
// 3D JavaScript used by media9.sty
//
// Extended functionality of the (right click) context menu of 3D annotations.
//
//  1.) Adds the following items to the 3D context menu:
//
//   * `Generate Default View'
//
//      Finds good default camera settings, returned as options for use with
//      the \includemedia command.
//
//   * `Get Current View'
//
//      Determines camera, cross section and part settings of the current view,
//      returned as `VIEW' section that can be copied into a views file of
//      additional views. The views file is inserted using the `3Dviews' option
//      of \includemedia.
//
//   * `Cross Section'
//
//      Toggle switch to add or remove a cross section into or from the current
//      view. The cross section can be moved in the x, y, z directions using x,
//      y, z and X, Y, Z keys on the keyboard, be tilted against and spun
//      around the upright Z axis using the Up/Down and Left/Right arrow keys
//      and caled using the s and S keys.
//
//  2.) Enables manipulation of position and orientation of indiviual parts and
//      groups of parts in the 3D scene. Parts which have been selected with the
//      mouse can be scaled moved around and rotated like the cross section as
//      described above. To spin the parts around their local up-axis, keep
//      Control key pressed while using the Up/Down and Left/Right arrow keys.
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
// The code borrows heavily from Bernd Gaertners `Miniball' software,
// originally written in C++, for computing the smallest enclosing ball of a
// set of points; see: http://www.inf.ethz.ch/personal/gaertner/miniball.html
//
////////////////////////////////////////////////////////////////////////////////
//host.console.show();

//constructor for doubly linked list
function List(){
  this.first_node=null;
  this.last_node=new Node(undefined);
}
List.prototype.push_back=function(x){
  var new_node=new Node(x);
  if(this.first_node==null){
    this.first_node=new_node;
    new_node.prev=null;
  }else{
    new_node.prev=this.last_node.prev;
    new_node.prev.next=new_node;
  }
  new_node.next=this.last_node;
  this.last_node.prev=new_node;
};
List.prototype.move_to_front=function(it){
  var node=it.get();
  if(node.next!=null && node.prev!=null){
    node.next.prev=node.prev;
    node.prev.next=node.next;
    node.prev=null;
    node.next=this.first_node;
    this.first_node.prev=node;
    this.first_node=node;
  }
};
List.prototype.begin=function(){
  var i=new Iterator();
  i.target=this.first_node;
  return(i);
};
List.prototype.end=function(){
  var i=new Iterator();
  i.target=this.last_node;
  return(i);
};
function Iterator(it){
  if( it!=undefined ){
    this.target=it.target;
  }else {
    this.target=null;
  }
}
Iterator.prototype.set=function(it){this.target=it.target;};
Iterator.prototype.get=function(){return(this.target);};
Iterator.prototype.deref=function(){return(this.target.data);};
Iterator.prototype.incr=function(){
  if(this.target.next!=null) this.target=this.target.next;
};
//constructor for node objects that populate the linked list
function Node(x){
  this.prev=null;
  this.next=null;
  this.data=x;
}
function sqr(r){return(r*r);}//helper function

//Miniball algorithm by B. Gaertner
function Basis(){
  this.m=0;
  this.q0=new Array(3);
  this.z=new Array(4);
  this.f=new Array(4);
  this.v=new Array(new Array(3), new Array(3), new Array(3), new Array(3));
  this.a=new Array(new Array(3), new Array(3), new Array(3), new Array(3));
  this.c=new Array(new Array(3), new Array(3), new Array(3), new Array(3));
  this.sqr_r=new Array(4);
  this.current_c=this.c[0];
  this.current_sqr_r=0;
  this.reset();
}
Basis.prototype.center=function(){return(this.current_c);};
Basis.prototype.size=function(){return(this.m);};
Basis.prototype.pop=function(){--this.m;};
Basis.prototype.excess=function(p){
  var e=-this.current_sqr_r;
  for(var k=0;k<3;++k){
    e+=sqr(p[k]-this.current_c[k]);
  }
  return(e);
};
Basis.prototype.reset=function(){
  this.m=0;
  for(var j=0;j<3;++j){
    this.c[0][j]=0;
  }
  this.current_c=this.c[0];
  this.current_sqr_r=-1;
};
Basis.prototype.push=function(p){
  var i, j;
  var eps=1e-32;
  if(this.m==0){
    for(i=0;i<3;++i){
      this.q0[i]=p[i];
    }
    for(i=0;i<3;++i){
      this.c[0][i]=this.q0[i];
    }
    this.sqr_r[0]=0;
  }else {
    for(i=0;i<3;++i){
      this.v[this.m][i]=p[i]-this.q0[i];
    }
    for(i=1;i<this.m;++i){
      this.a[this.m][i]=0;
      for(j=0;j<3;++j){
        this.a[this.m][i]+=this.v[i][j]*this.v[this.m][j];
      }
      this.a[this.m][i]*=(2/this.z[i]);
    }
    for(i=1;i<this.m;++i){
      for(j=0;j<3;++j){
        this.v[this.m][j]-=this.a[this.m][i]*this.v[i][j];
      }
    }
    this.z[this.m]=0;
    for(j=0;j<3;++j){
      this.z[this.m]+=sqr(this.v[this.m][j]);
    }
    this.z[this.m]*=2;
    if(this.z[this.m]<eps*this.current_sqr_r) return(false);
    var e=-this.sqr_r[this.m-1];
    for(i=0;i<3;++i){
      e+=sqr(p[i]-this.c[this.m-1][i]);
    }
    this.f[this.m]=e/this.z[this.m];
    for(i=0;i<3;++i){
      this.c[this.m][i]=this.c[this.m-1][i]+this.f[this.m]*this.v[this.m][i];
    }
    this.sqr_r[this.m]=this.sqr_r[this.m-1]+e*this.f[this.m]/2;
  }
  this.current_c=this.c[this.m];
  this.current_sqr_r=this.sqr_r[this.m];
  ++this.m;
  return(true);
};
function Miniball(){
  this.L=new List();
  this.B=new Basis();
  this.support_end=new Iterator();
}
Miniball.prototype.mtf_mb=function(it){
  var i=new Iterator(it);
  this.support_end.set(this.L.begin());
  if((this.B.size())==4) return;
  for(var k=new Iterator(this.L.begin());k.get()!=i.get();){
    var j=new Iterator(k);
    k.incr();
    if(this.B.excess(j.deref()) > 0){
      if(this.B.push(j.deref())){
        this.mtf_mb(j);
        this.B.pop();
        if(this.support_end.get()==j.get())
          this.support_end.incr();
        this.L.move_to_front(j);
      }
    }
  }
};
Miniball.prototype.check_in=function(b){
  this.L.push_back(b);
};
Miniball.prototype.build=function(){
  this.B.reset();
  this.support_end.set(this.L.begin());
  this.mtf_mb(this.L.end());
};
Miniball.prototype.center=function(){
  return(this.B.center());
};
Miniball.prototype.radius=function(){
  return(Math.sqrt(this.B.current_sqr_r));
};

//functions called by menu items
function calc3Dopts () {
  //create Miniball object
  var mb=new Miniball();
  //auxiliary vector
  var corner=new Vector3();
  //iterate over all visible mesh nodes in the scene
  for(i=0;i<scene.meshes.count;i++){
    var mesh=scene.meshes.getByIndex(i);
    if(!mesh.visible) continue;
    //local to parent transformation matrix
    var trans=mesh.transform;
    //build local to world transformation matrix by recursively
    //multiplying the parent's transf. matrix on the right
    var parent=mesh.parent;
    while(parent.transform){
      trans=trans.multiply(parent.transform);
      parent=parent.parent;
    }
    //get the bbox of the mesh (local coordinates)
    var bbox=mesh.computeBoundingBox();
    //transform the local bounding box corner coordinates to
    //world coordinates for bounding sphere determination
    //BBox.min
    corner.set(bbox.min);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    //BBox.max
    corner.set(bbox.max);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    //remaining six BBox corners
    corner.set(bbox.min.x, bbox.max.y, bbox.max.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    corner.set(bbox.min.x, bbox.min.y, bbox.max.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    corner.set(bbox.min.x, bbox.max.y, bbox.min.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    corner.set(bbox.max.x, bbox.min.y, bbox.min.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    corner.set(bbox.max.x, bbox.min.y, bbox.max.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
    corner.set(bbox.max.x, bbox.max.y, bbox.min.z);
    corner.set(trans.transformPosition(corner));
    mb.check_in(new Array(corner.x, corner.y, corner.z));
  }
  //compute the smallest enclosing bounding sphere
  mb.build();
  //
  //current camera settings
  //
  var camera=scene.cameras.getByIndex(0);
  var res=''; //initialize result string
  //aperture angle of the virtual camera (perspective projection) *or*
  //orthographic scale (orthographic projection)
  if(camera.projectionType==camera.TYPE_PERSPECTIVE){
    var aac=camera.fov*180/Math.PI;
    if(host.util.printf('%.4f', aac)!=30)
      res+=host.util.printf('\n3Daac=%s,', aac);
  }else{
      camera.viewPlaneSize=2.*mb.radius();
      res+=host.util.printf('\n3Dortho=%s,', 1./camera.viewPlaneSize);
  }
  //camera roll
  var roll = camera.roll*180/Math.PI;
  if(host.util.printf('%.4f', roll)!=0)
    res+=host.util.printf('\n3Droll=%s,',roll);
  //target to camera vector
  var c2c=new Vector3();
  c2c.set(camera.position);
  c2c.subtractInPlace(camera.targetPosition);
  c2c.normalize();
  if(!(c2c.x==0 && c2c.y==-1 && c2c.z==0))
    res+=host.util.printf('\n3Dc2c=%s %s %s,', c2c.x, c2c.y, c2c.z);
  //
  //new camera settings
  //
  //bounding sphere centre --> new camera target
  var coo=new Vector3();
  coo.set((mb.center())[0], (mb.center())[1], (mb.center())[2]);
  if(coo.length)
    res+=host.util.printf('\n3Dcoo=%s %s %s,', coo.x, coo.y, coo.z);
  //radius of orbit
  if(camera.projectionType==camera.TYPE_PERSPECTIVE){
    var roo=mb.radius()/ Math.sin(aac * Math.PI/ 360.);
  }else{
    //orthographic projection
    var roo=mb.radius();
  }
  res+=host.util.printf('\n3Droo=%s,', roo);
  //update camera settings in the viewer
  var currol=camera.roll;
  camera.targetPosition.set(coo);
  camera.position.set(coo.add(c2c.scale(roo)));
  camera.roll=currol;
  //determine background colour
  rgb=scene.background.getColor();
  if(!(rgb.r==1 && rgb.g==1 && rgb.b==1))
    res+=host.util.printf('\n3Dbg=%s %s %s,', rgb.r, rgb.g, rgb.b);
  //determine lighting scheme
  switch(scene.lightScheme){
    case scene.LIGHT_MODE_FILE:
      curlights='Artwork';break;
    case scene.LIGHT_MODE_NONE:
      curlights='None';break;
    case scene.LIGHT_MODE_WHITE:
      curlights='White';break;
    case scene.LIGHT_MODE_DAY:
      curlights='Day';break;
    case scene.LIGHT_MODE_NIGHT:
      curlights='Night';break;
    case scene.LIGHT_MODE_BRIGHT:
      curlights='Hard';break;
    case scene.LIGHT_MODE_RGB:
      curlights='Primary';break;
    case scene.LIGHT_MODE_BLUE:
      curlights='Blue';break;
    case scene.LIGHT_MODE_RED:
      curlights='Red';break;
    case scene.LIGHT_MODE_CUBE:
      curlights='Cube';break;
    case scene.LIGHT_MODE_CAD:
      curlights='CAD';break;
    case scene.LIGHT_MODE_HEADLAMP:
      curlights='Headlamp';break;
  }
  if(curlights!='Artwork')
    res+=host.util.printf('\n3Dlights=%s,', curlights);
  //determine global render mode
  switch(scene.renderMode){
    case scene.RENDER_MODE_BOUNDING_BOX:
      currender='BoundingBox';break;
    case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX:
      currender='TransparentBoundingBox';break;
    case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX_OUTLINE:
      currender='TransparentBoundingBoxOutline';break;
    case scene.RENDER_MODE_VERTICES:
      currender='Vertices';break;
    case scene.RENDER_MODE_SHADED_VERTICES:
      currender='ShadedVertices';break;
    case scene.RENDER_MODE_WIREFRAME:
      currender='Wireframe';break;
    case scene.RENDER_MODE_SHADED_WIREFRAME:
      currender='ShadedWireframe';break;
    case scene.RENDER_MODE_SOLID:
      currender='Solid';break;
    case scene.RENDER_MODE_TRANSPARENT:
      currender='Transparent';break;
    case scene.RENDER_MODE_SOLID_WIREFRAME:
      currender='SolidWireframe';break;
    case scene.RENDER_MODE_TRANSPARENT_WIREFRAME:
      currender='TransparentWireframe';break;
    case scene.RENDER_MODE_ILLUSTRATION:
      currender='Illustration';break;
    case scene.RENDER_MODE_SOLID_OUTLINE:
      currender='SolidOutline';break;
    case scene.RENDER_MODE_SHADED_ILLUSTRATION:
      currender='ShadedIllustration';break;
    case scene.RENDER_MODE_HIDDEN_WIREFRAME:
      currender='HiddenWireframe';break;
  }
  if(currender!='Solid')
    res+=host.util.printf('\n3Drender=%s,', currender);
  //write result string to the console
  host.console.show();
//  host.console.clear();
  host.console.println('%%\n%% Copy and paste the following text to the\n'+
    '%% option list of \\includemedia!\n%%' + res + '\n');
}

function get3Dview () {
  var camera=scene.cameras.getByIndex(0);
  var coo=camera.targetPosition;
  var c2c=camera.position.subtract(coo);
  var roo=c2c.length;
  c2c.normalize();
  var res='VIEW%=insert optional name here\n';
  if(!(coo.x==0 && coo.y==0 && coo.z==0))
    res+=host.util.printf('  COO=%s %s %s\n', coo.x, coo.y, coo.z);
  if(!(c2c.x==0 && c2c.y==-1 && c2c.z==0))
    res+=host.util.printf('  C2C=%s %s %s\n', c2c.x, c2c.y, c2c.z);
  if(roo > 1e-9)
    res+=host.util.printf('  ROO=%s\n', roo);
  var roll = camera.roll*180/Math.PI;
  if(host.util.printf('%.4f', roll)!=0)
    res+=host.util.printf('  ROLL=%s\n', roll);
  if(camera.projectionType==camera.TYPE_PERSPECTIVE){
    var aac=camera.fov * 180/Math.PI;
    if(host.util.printf('%.4f', aac)!=30)
      res+=host.util.printf('  AAC=%s\n', aac);
  }else{
    if(host.util.printf('%.4f', camera.viewPlaneSize)!=1)
      res+=host.util.printf('  ORTHO=%s\n', 1./camera.viewPlaneSize);
  }
  rgb=scene.background.getColor();
  if(!(rgb.r==1 && rgb.g==1 && rgb.b==1))
    res+=host.util.printf('  BGCOLOR=%s %s %s\n', rgb.r, rgb.g, rgb.b);
  switch(scene.lightScheme){
    case scene.LIGHT_MODE_FILE:
      curlights='Artwork';break;
    case scene.LIGHT_MODE_NONE:
      curlights='None';break;
    case scene.LIGHT_MODE_WHITE:
      curlights='White';break;
    case scene.LIGHT_MODE_DAY:
      curlights='Day';break;
    case scene.LIGHT_MODE_NIGHT:
      curlights='Night';break;
    case scene.LIGHT_MODE_BRIGHT:
      curlights='Hard';break;
    case scene.LIGHT_MODE_RGB:
      curlights='Primary';break;
    case scene.LIGHT_MODE_BLUE:
      curlights='Blue';break;
    case scene.LIGHT_MODE_RED:
      curlights='Red';break;
    case scene.LIGHT_MODE_CUBE:
      curlights='Cube';break;
    case scene.LIGHT_MODE_CAD:
      curlights='CAD';break;
    case scene.LIGHT_MODE_HEADLAMP:
      curlights='Headlamp';break;
  }
  if(curlights!='Artwork')
    res+='  LIGHTS='+curlights+'\n';
  switch(scene.renderMode){
    case scene.RENDER_MODE_BOUNDING_BOX:
      defaultrender='BoundingBox';break;
    case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX:
      defaultrender='TransparentBoundingBox';break;
    case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX_OUTLINE:
      defaultrender='TransparentBoundingBoxOutline';break;
    case scene.RENDER_MODE_VERTICES:
      defaultrender='Vertices';break;
    case scene.RENDER_MODE_SHADED_VERTICES:
      defaultrender='ShadedVertices';break;
    case scene.RENDER_MODE_WIREFRAME:
      defaultrender='Wireframe';break;
    case scene.RENDER_MODE_SHADED_WIREFRAME:
      defaultrender='ShadedWireframe';break;
    case scene.RENDER_MODE_SOLID:
      defaultrender='Solid';break;
    case scene.RENDER_MODE_TRANSPARENT:
      defaultrender='Transparent';break;
    case scene.RENDER_MODE_SOLID_WIREFRAME:
      defaultrender='SolidWireframe';break;
    case scene.RENDER_MODE_TRANSPARENT_WIREFRAME:
      defaultrender='TransparentWireframe';break;
    case scene.RENDER_MODE_ILLUSTRATION:
      defaultrender='Illustration';break;
    case scene.RENDER_MODE_SOLID_OUTLINE:
      defaultrender='SolidOutline';break;
    case scene.RENDER_MODE_SHADED_ILLUSTRATION:
      defaultrender='ShadedIllustration';break;
    case scene.RENDER_MODE_HIDDEN_WIREFRAME:
      defaultrender='HiddenWireframe';break;
  }
  if(defaultrender!='Solid')
    res+='  RENDERMODE='+defaultrender+'\n';

  //detect existing Clipping Plane (3D Cross Section)
  var clip=null;
  if(
    clip=scene.nodes.getByName('$$$$$$')||
    clip=scene.nodes.getByName('Clipping Plane')
  );
  for(var i=0;i<scene.nodes.count;i++){
    var nd=scene.nodes.getByIndex(i);
    if(nd==clip||nd.name=='') continue;
    var ndUTFName='';
    for (var j=0; j<nd.name.length; j++) {
      var theUnicode = nd.name.charCodeAt(j).toString(16);
      while (theUnicode.length<4) theUnicode = '0' + theUnicode;
      ndUTFName += theUnicode;
    }
    var end=nd.name.lastIndexOf('.');
    if(end>0) var ndUserName=nd.name.substr(0,end);
    else var ndUserName=nd.name;
    respart='  PART='+ndUserName+'\n';
    respart+='    UTF16NAME='+ndUTFName+'\n';
    defaultvals=true;
    if(!nd.visible){
      respart+='    VISIBLE=false\n';
      defaultvals=false;
    }
    if(nd.opacity<1.0){
      respart+='    OPACITY='+nd.opacity+'\n';
      defaultvals=false;
    }
    if(nd.constructor.name=='Mesh'){
      currender=defaultrender;
      switch(nd.renderMode){
        case scene.RENDER_MODE_BOUNDING_BOX:
          currender='BoundingBox';break;
        case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX:
          currender='TransparentBoundingBox';break;
        case scene.RENDER_MODE_TRANSPARENT_BOUNDING_BOX_OUTLINE:
          currender='TransparentBoundingBoxOutline';break;
        case scene.RENDER_MODE_VERTICES:
          currender='Vertices';break;
        case scene.RENDER_MODE_SHADED_VERTICES:
          currender='ShadedVertices';break;
        case scene.RENDER_MODE_WIREFRAME:
          currender='Wireframe';break;
        case scene.RENDER_MODE_SHADED_WIREFRAME:
          currender='ShadedWireframe';break;
        case scene.RENDER_MODE_SOLID:
          currender='Solid';break;
        case scene.RENDER_MODE_TRANSPARENT:
          currender='Transparent';break;
        case scene.RENDER_MODE_SOLID_WIREFRAME:
          currender='SolidWireframe';break;
        case scene.RENDER_MODE_TRANSPARENT_WIREFRAME:
          currender='TransparentWireframe';break;
        case scene.RENDER_MODE_ILLUSTRATION:
          currender='Illustration';break;
        case scene.RENDER_MODE_SOLID_OUTLINE:
          currender='SolidOutline';break;
        case scene.RENDER_MODE_SHADED_ILLUSTRATION:
          currender='ShadedIllustration';break;
        case scene.RENDER_MODE_HIDDEN_WIREFRAME:
          currender='HiddenWireframe';break;
        //case scene.RENDER_MODE_DEFAULT:
        //  currender='Default';break;
      }
      if(currender!=defaultrender){
        respart+='    RENDERMODE='+currender+'\n';
        defaultvals=false;
      }
    }
    if(origtrans[nd.name]&&!nd.transform.isEqual(origtrans[nd.name])){
      var lvec=nd.transform.transformDirection(new Vector3(1,0,0));
      var uvec=nd.transform.transformDirection(new Vector3(0,1,0));
      var vvec=nd.transform.transformDirection(new Vector3(0,0,1));
      respart+='    TRANSFORM='
               +lvec.x+' '+lvec.y+' '+lvec.z+' '
               +uvec.x+' '+uvec.y+' '+uvec.z+' '
               +vvec.x+' '+vvec.y+' '+vvec.z+' '
               +nd.transform.translation.x+' '
               +nd.transform.translation.y+' '
               +nd.transform.translation.z+'\n';
      defaultvals=false;
    }
    respart+='  END\n';
    if(!defaultvals) res+=respart;
  }
  if(clip){
    var centre=clip.transform.translation;
    var normal=clip.transform.transformDirection(new Vector3(0,0,1));
    res+='  CROSSSECT\n';
    if(!(centre.x==0 && centre.y==0 && centre.z==0))
      res+=host.util.printf(
        '    CENTER=%s %s %s\n', centre.x, centre.y, centre.z);
    if(!(normal.x==1 && normal.y==0 && normal.z==0))
      res+=host.util.printf(
        '    NORMAL=%s %s %s\n', normal.x, normal.y, normal.z);
    res+=host.util.printf(
      '    VISIBLE=%s\n', clip.visible);
    res+=host.util.printf(
      '    PLANECOLOR=%s %s %s\n', clip.material.emissiveColor.r,
             clip.material.emissiveColor.g, clip.material.emissiveColor.b);
    res+=host.util.printf(
      '    OPACITY=%s\n', clip.opacity);
    res+=host.util.printf(
      '    INTERSECTIONCOLOR=%s %s %s\n',
        clip.wireframeColor.r, clip.wireframeColor.g, clip.wireframeColor.b);
    res+='  END\n';
//    for(var propt in clip){
//      console.println(propt+':'+clip[propt]);
//    }
  }
  res+='END\n';
  host.console.show();
//  host.console.clear();
  host.console.println('%%\n%% Add the following VIEW section to a file of\n'+
    '%% predefined views (See option "3Dviews"!).\n%%\n' +
    '%% The view may be given a name after VIEW=...\n' +
    '%% (Remove \'%\' in front of \'=\'.)\n%%');
  host.console.println(res + '\n');
}

//add items to 3D context menu
runtime.addCustomMenuItem("dfltview", "Generate Default View", "default", 0);
runtime.addCustomMenuItem("currview", "Get Current View", "default", 0);
runtime.addCustomMenuItem("csection", "Cross Section", "checked", 0);

//menu event handlers
menuEventHandler = new MenuEventHandler();
menuEventHandler.onEvent = function(e) {
  switch(e.menuItemName){
    case "dfltview": calc3Dopts(); break;
    case "currview": get3Dview(); break;
    case "csection":
      addremoveClipPlane(e.menuItemChecked);
      break;
  }
};
runtime.addEventHandler(menuEventHandler);

//global variable taking reference to currently selected node;
var target=null;
selectionEventHandler=new SelectionEventHandler();
selectionEventHandler.onEvent=function(e){
  if(e.selected&&e.node.name!=''){
    target=e.node;
  }else{
    target=null;
  }
}
runtime.addEventHandler(selectionEventHandler);

cameraEventHandler=new CameraEventHandler();
cameraEventHandler.onEvent=function(e){
  var clip=null;
  runtime.removeCustomMenuItem("csection");
  runtime.addCustomMenuItem("csection", "Cross Section", "checked", 0);
  if(clip=scene.nodes.getByName('$$$$$$')|| //predefined
    scene.nodes.getByName('Clipping Plane')){ //added via context menu
    runtime.removeCustomMenuItem("csection");
    runtime.addCustomMenuItem("csection", "Cross Section", "checked", 1);
  }
  if(clip){//plane in predefined views must be rotated by 90 deg around normal
    clip.transform.rotateAboutLineInPlace(
      Math.PI/2,clip.transform.translation,
      clip.transform.transformDirection(new Vector3(0,0,1))
    );
  }
  for(var i=0; i<rot4x4.length; i++){rot4x4[i].setIdentity()}
  target=null;
}
runtime.addEventHandler(cameraEventHandler);

var rot4x4=new Array(); //keeps track of spin and tilt axes transformations
//key event handler for scaling moving, spinning and tilting objects
keyEventHandler=new KeyEventHandler();
keyEventHandler.onEvent=function(e){
  var backtrans=new Matrix4x4();
  var trgt=null;
  if(target) {
    trgt=target;
    var backtrans=new Matrix4x4();
    var trans=trgt.transform;
    var parent=trgt.parent;
    while(parent.transform){
      //build local to world transformation matrix
      trans.multiplyInPlace(parent.transform);
      //also build world to local back-transformation matrix
      backtrans.multiplyInPlace(parent.transform.inverse.transpose);
      parent=parent.parent;
    }
    backtrans.transposeInPlace();
  }else{
    if(
      trgt=scene.nodes.getByName('$$$$$$')||
      trgt=scene.nodes.getByName('Clipping Plane')
    ) var trans=trgt.transform;
  }
  if(!trgt) return;

  var tname=trgt.name;
  if(typeof(rot4x4[tname])=='undefined') rot4x4[tname]=new Matrix4x4();
  if(target)
    var tiltAxis=rot4x4[tname].transformDirection(new Vector3(0,1,0));
  else  
    var tiltAxis=trans.transformDirection(new Vector3(0,1,0));
  var spinAxis=rot4x4[tname].transformDirection(new Vector3(0,0,1));

  //get the centre of the mesh
  if(target&&trgt.constructor.name=='Mesh'){
    var centre=trans.transformPosition(trgt.computeBoundingBox().center);
  }else{ //part group (Node3 parent node, clipping plane)
    var centre=new Vector3(trans.translation);
  }
  switch(e.characterCode){
    case 30://tilt up
      rot4x4[tname].rotateAboutLineInPlace(
          -Math.PI/900,rot4x4[tname].translation,tiltAxis);
      trans.rotateAboutLineInPlace(-Math.PI/900,centre,tiltAxis);
      break;
    case 31://tilt down
      rot4x4[tname].rotateAboutLineInPlace(
          Math.PI/900,rot4x4[tname].translation,tiltAxis);
      trans.rotateAboutLineInPlace(Math.PI/900,centre,tiltAxis);
      break;
    case 28://spin right
      if(e.ctrlKeyDown&&target){
        trans.rotateAboutLineInPlace(-Math.PI/900,centre,spinAxis);
      }else{
        rot4x4[tname].rotateAboutLineInPlace(
            -Math.PI/900,rot4x4[tname].translation,new Vector3(0,0,1));
        trans.rotateAboutLineInPlace(-Math.PI/900,centre,new Vector3(0,0,1));
      }
      break;
    case 29://spin left
      if(e.ctrlKeyDown&&target){
        trans.rotateAboutLineInPlace(Math.PI/900,centre,spinAxis);
      }else{
        rot4x4[tname].rotateAboutLineInPlace(
            Math.PI/900,rot4x4[tname].translation,new Vector3(0,0,1));
        trans.rotateAboutLineInPlace(Math.PI/900,centre,new Vector3(0,0,1));
      }
      break;
    case 120: //x
      translateTarget(trans, new Vector3(1,0,0), e);
      break;
    case 121: //y
      translateTarget(trans, new Vector3(0,1,0), e);
      break;
    case 122: //z
      translateTarget(trans, new Vector3(0,0,1), e);
      break;
    case 88: //shift + x
      translateTarget(trans, new Vector3(-1,0,0), e);
      break;
    case 89: //shift + y
      translateTarget(trans, new Vector3(0,-1,0), e);
      break;
    case 90: //shift + z
      translateTarget(trans, new Vector3(0,0,-1), e);
      break;
    case 115: //s
      trans.translateInPlace(centre.scale(-1));
      trans.scaleInPlace(1.01);
      trans.translateInPlace(centre.scale(1));
      break;
    case 83: //shift + s
      trans.translateInPlace(centre.scale(-1));
      trans.scaleInPlace(1/1.01);
      trans.translateInPlace(centre.scale(1));
      break;
  }
  trans.multiplyInPlace(backtrans);
}
runtime.addEventHandler(keyEventHandler);

//translates object by amount calculated from Canvas size
function translateTarget(t, d, e){
  var cam=scene.cameras.getByIndex(0);
  if(cam.projectionType==cam.TYPE_PERSPECTIVE){
    var scale=Math.tan(cam.fov/2)
              *cam.targetPosition.subtract(cam.position).length
              /Math.min(e.canvasPixelWidth,e.canvasPixelHeight);
  }else{
    var scale=cam.viewPlaneSize/2
              /Math.min(e.canvasPixelWidth,e.canvasPixelHeight);
  }
  t.translateInPlace(d.scale(scale));
}

function addremoveClipPlane(chk) {
  var curTrans=getCurTrans();
  var clip=scene.createClippingPlane();
  if(chk){
    //add Clipping Plane and place its center either into the camera target
    //position or into the centre of the currently selected mesh node
    var centre=new Vector3();
    if(target){
      var trans=target.transform;
      var parent=target.parent;
      while(parent.transform){
        trans=trans.multiply(parent.transform);
        parent=parent.parent;
      }
      if(target.constructor.name=='Mesh'){
        var centre=trans.transformPosition(target.computeBoundingBox().center);
      }else{
        var centre=new Vector3(trans.translation);
      }
      target=null;
    }else{
      centre.set(scene.cameras.getByIndex(0).targetPosition);
    }
    clip.transform.setView(
      new Vector3(0,0,0), new Vector3(1,0,0), new Vector3(0,1,0));
    clip.transform.translateInPlace(centre);
  }else{
    if(
      scene.nodes.getByName('$$$$$$')||
      scene.nodes.getByName('Clipping Plane')
    ){
      clip.remove();clip=null;
    }
  }
  restoreTrans(curTrans);
  return clip;
}

//function to store current transformation matrix of all nodes in the scene
function getCurTrans() {
  var tA=new Array();
  for(var i=0; i<scene.nodes.count; i++){
    var nd=scene.nodes.getByIndex(i);
    if(nd.name=='') continue;
    tA[nd.name]=new Matrix4x4(nd.transform);
  }
  return tA;
}

//function to restore transformation matrices given as arg
function restoreTrans(tA) {
  for(var i=0; i<scene.nodes.count; i++){
    var nd=scene.nodes.getByIndex(i);
    if(tA[nd.name]) nd.transform.set(tA[nd.name]);
  }
}

//store original transformation matrix of all mesh nodes in the scene
var origtrans=getCurTrans();

//set initial state of "Cross Section" menu entry
cameraEventHandler.onEvent(1);

//host.console.clear();
