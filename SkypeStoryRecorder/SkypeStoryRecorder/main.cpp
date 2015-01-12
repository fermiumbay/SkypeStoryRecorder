#include "SkypeStoryRecorder.h"

const bool txtStoryOutputFlg = true;	// �ł����������X�g�[���[�t�@�C�����A�{��txt�Ƃ��ďo�͂��邩�H
const bool txtDetailOutputFlg = true;	// �ł����������X�g�[���[�t�@�C�����A�ڍ�txt�Ƃ��ďo�͂��邩�H

int main(int argc, char* argv[]){
	if (argc > 2){
		cout << "�R�}���h�̓��͂��������܂��I" << endl;
		return 1;
	}
	string path = (argc == 1) ?
		"" : static_cast<string>(argv[1]);
	string buf;

	do{
		SkypeStoryRecorder ssr(txtStoryOutputFlg, txtDetailOutputFlg);
		bool success = ssr.Run();

		cout << "[�������������܂���]" << endl;
		if (path != "" && success){
			break;
		}
		cout << "  - ������x���s����ɂ�y�����" << endl;
		cout << "  - �I������ꍇ�͂���ȊO�̕��������" << endl;
		cout << "    : ";
		cin >> buf;
	} while (buf == "y");
	if (path != ""){
		string command = "start " + path;
		system(command.c_str());	// �����Ƀp�X������΁A�I���セ�̃v���O���������s
	}
	return 0;
}
