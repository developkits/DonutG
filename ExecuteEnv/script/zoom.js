//��ʂ̃Y�[�����s���X�N���v�g
//���s�������Ƃ���0.1�`10�̒l��n�����Ƃ�10%�`1000%
//�̊Ԃŕ\���T�C�Y��ύX�ł���

var Donut = new ActiveXObject("DonutP.API");
var document = Donut.GetDocumentObject(Donut.TabIndex);

var nZoomSize = parseFloat(WScript.Arguments(0));
if(nZoomSize < 0.1 || 10.0 < nZoomSize){
	nZoomSize = 1.0;
}

document.body.style.zoom=nZoomSize;