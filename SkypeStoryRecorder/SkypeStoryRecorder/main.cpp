#include "SkypeStoryRecorder.h"

const bool txtStoryOutputFlg = true;	// できあがったストーリーファイルを、本文txtとして出力するか？
const bool txtDetailOutputFlg = true;	// できあがったストーリーファイルを、詳細txtとして出力するか？

int main(int argc, char* argv[]){
	if (argc > 2){
		cout << "コマンドの入力が多すぎます！" << endl;
		return 1;
	}
	string path = (argc == 1) ?
		"" : static_cast<string>(argv[1]);
	string buf;

	do{
		SkypeStoryRecorder ssr(txtStoryOutputFlg, txtDetailOutputFlg);
		bool success = ssr.Run();

		cout << "[処理が完了しました]" << endl;
		if (path != "" && success){
			break;
		}
		cout << "  - もう一度実行するにはyを入力" << endl;
		cout << "  - 終了する場合はそれ以外の文字を入力" << endl;
		cout << "    : ";
		cin >> buf;
	} while (buf == "y");
	if (path != ""){
		string command = "start " + path;
		system(command.c_str());	// 引数にパスがあれば、終了後そのプログラムを実行
	}
	return 0;
}
