//Crash test for QS script

function _crash() {
 //Operator crashes
 x=createEmptyOperator();
 y=x.getName();
 x.iterator();

 //Put any code that crashes script here for future semi-automatic testing 

 print('SUCCESS');
}

function _crash_treeitem() {
 try {
  parentType=treeitem.parent().itemtype();
  if (parentType!="Dict" && parentType!="Array") throw "Parent of selected tree item must be Dict/Array";
 } catch (e) {
  print("Error"+e);
  return;
 }
 x=treeitem;
 if (!x.valid()) throw "!x.valid()";
 treeitem.remove();
 if (x.valid()) throw "x.valid()";
 print('SUCCESS');
}