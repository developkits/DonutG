'LinkExtractor�v���O�C���͈��(0�Ԗ�)�̈����Ƃ���URL�ꗗ�t�@�C���̃t���p�X���A
'���(1�Ԗ�)�̈����Ƃ���URL�̌���^���ăX�N���v�g���Ăяo���܂��B
'�����̈����ɂ�WScript.Argument(i)����A�N�Z�X�ł��܂��B
'URL�ꗗ�t�@�C����URL���ɉ��s�ŋ�؂��Ă��܂��B
'�����̏��𗘗p���āA�_�E�����[�_��URL�𑗂�̂��X�N���v�g�̖����ł��B'

On Error Resume Next
Set objArg = WScript.Arguments
Set Irvine = CreateObject("Irvine.Api")
If err=0 And objArg.Count >= 1 Then
	Set objFileSystem = CreateObject("Scripting.FileSystemObject\")
	Set objFile       = objFileSystem.OpenTextFile(objArg(0))
	Dim strURLs
	strURLs = objFile.ReadAll()
	call Irvine.AddUrl(strURLs,1)
End If