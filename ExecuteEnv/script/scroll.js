//���s�������Ɏw�肵���ʂ����X�N���[��

var Donut = new ActiveXObject("DonutP.API");
var nIndex = Donut.TabIndex;
var window = Donut.GetWindowObject(nIndex);

var amount = eval(WScript.Arguments(0));
window.scrollBy(0,amount);