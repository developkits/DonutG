
var Donut, strIniPath, nCount;

//Donut�̋@�\�𑀍삷�邽�߂�Donut�I�u�W�F�N�g���擾����
Donut = new ActiveXObject("DonutP.API");

//API�o�^.bat�����s���Ă��Ȃ������肷��Ǝ擾�ł����A
//���̐�i�߂Ȃ��̂Œ��~����
if(Donut == null)
	WScript.Quit();

//�X�N���v�g�t�H���_�̃p�X���擾����ini�t�@�C���̃p�X���쐬����
strIniPath = Donut.GetScriptFolder();
if(strIniPath == "")
	WScript.Quit();
strIniPath += "\ini.ini";

//�ۑ����Ă���l��ǂݍ���ŁA1���₵�āA����ڂ��\�����āA�ۑ�����
nCount = Donut.GetProfileInt(strIniPath,"Test","Count",0);
nCount++;
WScript.Echo(nCount + "��ڂ̌Ăяo���ł�");
Donut.WriteProfileInt(strIniPath,"Test","Count",nCount);