//Java�̎��s�E����s��؂�ւ���

var DonutP, nIndex, nState;
var FLAG_SE_JAVA      = 0x00000040;
var FLAG_SE_NOREFRESH = 0x01000000; 

//Donut�̋@�\�𑀍삷�邽�߂�Donut�I�u�W�F�N�g���擾����
DonutP = new ActiveXObject("DonutP.API");

//�A�N�e�B�u�ȃ^�u�̃C���f�b�N�X�i�ʂ��ԍ��j���擾����
nIndex = DonutP.TabIndex;�@//�^�u���Ȃ��Ƃ���-1��Ԃ�

if(nIndex != -1){
	//�^�u�̏�Ԃ��擾����
	nState = DonutP.GetExtendedTabState(nIndex);

	if(nState & FLAG_SE_JAVA){
		//FLAG_SE_JAVA�̃t���O��OFF�ɂ���
		nState &= ~FLAG_SE_JAVA; 
	}else{
		//FLAG_SE_JAVA�̃t���O��ON�ɂ���
		nState |= FLAG_SE_JAVA;  
	}

    //�����[�h���Ȃ��悤�ɂ���
	nState |= FLAG_SE_NOREFRESH;

	//�V�����^�u�̏�Ԃ�ݒ肷��
	DonutP.SetExtendedTabState(nIndex,nState);
}


//
//�r�b�g����Ɋւ���ȒP�Ȑ���
//
//nState���^�u�̏�Ԃ̒l���Ƃ��܂��B
//
//����̃t���OA��ON�ɂ������Ƃ�
//nState |= A;
//
//����̃t���OA��OFF�ɂ������Ƃ�
//nState &= ~A;
//
//����̃t���OA�̏�Ԃ��t�ɂ������Ƃ�
//nState ^= A;
//
//����̃t���OA��ON�ł��邩�m�F�������Ƃ�
//nState & A
//�@��0�łȂ��Ƃ� : ON
//�@�@0�̂Ƃ��@�@ : OFF
//
//�����̃t���O�̏ꍇ�ł������ł��B
//�����̃t���OA,B,C�����ׂ�ON�ɂ������Ƃ�
//nState |= (A | B | C);
//