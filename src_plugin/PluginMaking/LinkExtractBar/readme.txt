LinkExtractBar.dll


undonut+mod�łł̏C�����e.

2008-07-07
�Evs2005/2008 �ŃR���p�C��. x64�����ǉ�.

�E������̏I�[�����Ɍ�肪����͈͊O�̃������ւ̏������݂��������̂��C��.

�E.dat�Ǎ����h���b�v�_�E���̂�����̒���

�E�_�E�����[�_�[�{�^���̏��Ԃ�ς���.
�Ewin64�łł̓_�E�����[�_�[�{�^����iria,irvine�̂�.
  ��iria,irvine��win32,win64���ɋ@�\���܂������A
�@�@DC�T�N��,FlushGet��win32�ł͋@�\�����
�@�@win64�ł͎��s���ɃC���^�[�t�F�[�X�擾�Ɏ��s�A
�@�@NetTransport�͔ł��V�����Ȃ肷�����̂�
�@�@win32,win64���ɃA�E�g�A�Ȃ̂ł����B

�E�K���ɃC���f���g����t������

2008-07-21
�ELinkExtructBar.dat(�g���q�Z�b�g) �� plugin�t�H���_����
�@�����łȂ��A���̒��� LinkExtructBar\ �t�H���_����ǂނ悤
�@�ɕύX�B

�@���l�� ToolBar.bmp(�c�[���o�[�A�C�R��)��LinkExtructBarRes\
�@�łȂ� LinkExtructBar\ ����ǂނ悤�ɕύX�B

�E�_�C�A���O�̐ݒ��Ԃ�ݒ�t�@�C���ɋL������悤�ɂ����B
   (����������,���K�\���ɂ��Ă̋L���̗L���̓`�F�b�N��on/off)
�@�t�@�C���́ALinkExtructBar.dll�������ꂽplugin�t�H���_����
�@LinkExtructBar\LinkExtructBar.ini �ł��B

�E�w�肵���_�E�����[�_�[�ɑ���̂�1�N���b�N�ōs���AS�{�^����
�@�����̂�p�ӁB
�@S�{�^���Ƀ`�F�b�N�����A�N���������_�E�����[�_��I�����āA
�@��Uundonut���I�����N������΃c�[���o�[��S�{�^���������܂��B
�@��S�{�^���̃`�F�b�N�����f�����̂�undonut�N�����̂�

�E�_�E�����[�_�[�̌Ăяo����{��undonut�� LinkExtructor.dll ��
�@�����ăX�N���v�g���Ăׂ�悤�ɕύX�B
�@�X�N���v�g(vba)��p�ӂ��Aplugin�t�H���_����LinkExtructBar\
�@�t�H���_�ɂ���A����ɁALinkExtructBar.ini ����
�@[Downloader]
�@0=irvine.vba
�@1=iria
�@�̂悤��0-5�̂��Âꂩ�ɁA�X�N���v�g�t�@�C�������g���q���݂�
�@�L���̂��ƁB

�@���������g���q�Ȃ��� Irvine,Iria,DcSakura �Ə������ꍇ�́A
�@�@���Ƃ��炠��������[�`���𗘗p�B

�@���{�^���͕K��6�\������A�L���ȓo�^�̂ݖ��O���o�܂��B
�@�@���L���̃{�^���͉����܂����A�������܂���B

�@�����݁Airvine,iria�����C���X�g�[�����Ă��Ȃ�����
�@�@���̃_�E�����[�_���N���ł��邩�ǂ����͖��m�F�ł��B


2008-07-25
�E�����N�ꗗ�ł̉E�N���b�N���j���[�ɃS�~(?)���ڂ��łĂ��̂��폜.
�E�_�C�A���O�̃T�C�Y��ݒ�t�@�C���ɕۑ�����悤�ɂ����B



���̑�

�EMyDialog.h����undonut.exe��_�E�����[�_�[���̃p�X���́A
�@�e�����Őݒ肵�Ȃ����Ă��������B
�@wtl��boost�̃p�X��ide���łȂ��Avcproj���̃p�X�ݒ�ōs���Ă���̂ŁA
�@���̂ւ���A�e���̃R���p�C�����ɂ��킹�ďC�����Ă��������B

�E�e��_�E�����[�_�̃^�C�v���C�u�����̃w�b�_�����Ă��܂��B
�@�K�v�ɉ����č�蒼�����ق����悢�ł��傤���A���������\�t�g��
�@�C���X�g�[�����Ȃ��Ƃ����Ȃ��̂��ʓ|�Ȃ̂Ŏ蔲�����Ă܂��B

�E���K�\�����C�u�����Ƃ��āAboost::regex���g���Ă���̂�
�@boost�̃C���X�g�[�����K�v�B���s�� ver. 1.35 �p�ɂ��킹�Ă���܂��B

�@win32�ł́A�R���p�C���ς݂̃p�b�P�[�W���z�z����Ă���̂ł���ł悢���ǁA
�@64�r�b�g�ł�bjam�ŗp�ӂ���K�v����(boost 1.35  2008-07-07 ����).
�@�R�}���h���C���� vcvarsx86_amd64.bat �����s���Ă�����
�@> bjam --toolset=msvc-9.0.amd64 runtime-link=static link=static debug release stage
�@�Ƃ��������B(.amd64�Ƃ� runtime-link�Ƃ��ɋC�Â����A��J�����̂Ń����j
�@(���̃v���O�C���ł̓����^�C����boost���ÓI�����N�������̂�static�w��̂�
�@�����ǕK�v�Ȃ�shared���C�u�������������Ă���)
�@�ڂ�����boost�̃C���X�g�[���̉���T�C�g���������Ċm�F���Ă��������B



2008-07-25  +mod
