//�A�N�e�B�u�^�u�̃^�C�g����URL���N���b�v�{�[�h�ɃR�s�[
var donut = Components.classes["@tnose.net/donut/api-service;1"]
    .getService(Components.interfaces.donutIAPI);

var nIndex = donut.tabIndex;
var document = donut.getDocumentObject(nIndex);

var text = document.title + "\n" + document.URL + "\n";

const gClipboardHelper = Components.classes["@mozilla.org/widget/clipboardhelper;1"].  
getService(Components.interfaces.nsIClipboardHelper);
gClipboardHelper.copyString(text);