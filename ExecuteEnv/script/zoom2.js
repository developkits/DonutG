//��ʂ̃Y�[�����s���X�N���v�g
//���s�������Ƃ���-5�`5�̒l��n�����Ƃ�-500%�`500%
//�̊Ԃŕ\���T�C�Y��ύX�ł���
//������͑��ΓI�ɃT�C�Y��ω�������

var Donut = new ActiveXObject("DonutP.API");
var document = Donut.GetDocumentObject(Donut.TabIndex);

var nZoomSize = parseFloat(WScript.Arguments(0));
if(nZoomSize < -5.0 || 5.0 < nZoomSize){
	nZoomSize = 0.0;
}

var present = parseFloat(document.body.style.zoom);
if(isNaN(present)) present = 1.0;
var nNewSize = present+nZoomSize;
if(nNewSize <= 0.0) nNewSize = 1.0;

document.body.style.zoom = nNewSize;
