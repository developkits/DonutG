//���s�������Ɏw�肵���ʂ����X�N���[��

var donut = Components.classes["@tnose.net/donut/api-service;1"]
    .getService(Components.interfaces.donutIAPI);

var nIndex = donut.tabIndex;
var window = donut.getWindowObject(nIndex);

var amount = 100;
window.scrollBy(0,amount);
