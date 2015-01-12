#include "TagData.h"

string TagData::GetTag(string buf){
	return buf.substr(1, buf.find(">") - 1);
};

TagData::TagData(ifstream &ifs){
	vector<TagData> tagData;

	while (!ifs.eof()){
		string buf;
		getline(ifs, buf);

		smatch match;
		regex_search(buf, match, regex("<.*>.*"));
		while (!match.empty()){
			string temp = *match.begin();
			string tag = GetTag(temp);
			if (!tagData.empty()){
				tagData[tagData.size() - 1].AddText(buf.substr(0, match.position()));
			}
			buf = temp.substr(tag.size() + 2);

			if (tag.substr(0, 1) != "/"){
				// タグを開くと、新規のタグデータを設定する
				tagData.push_back(TagData());
			}
			else{
				// タグを閉じると、そのタグデータを格納する
				tag = tag.substr(1);
				if (tagData.size() == 1){
					this->AddData(tag, tagData[0]);
				}
				else{
					tagData[tagData.size() - 2].AddData(tag, tagData[tagData.size() - 1]);
				}
				tagData.pop_back();
			}

			match = smatch();
			regex_search(buf, match, regex("<.*>.*"));
		}

		if (!tagData.empty()){
			tagData[tagData.size() - 1].AddText(buf + "\n");
		}
	}
}

TagData::TagData(string s) : text(s)
{
	data.clear();
}

TagData::TagData(const TagData& d){
	*this = d;
}

TagData& TagData::operator=(const TagData& d){
	this->data = d.data;
	this->text = d.text;
	return *this;
}

void TagData::AddData(string tag, TagData addData){
	bool addFlg = false;
	for (auto it = data.begin(); it != data.end(); it++){
		if (it->first == tag){
			it->second.push_back(addData);
			addFlg = true;
			break;
		}
	}
	if (!addFlg){
		vector<TagData> tmp;
		tmp.push_back(addData);
		data.push_back(pair<string, vector<TagData>>(tag, tmp));
	}
}

void TagData::AddData(string tag, string text){
	AddData(tag, TagData(text));
}

vector<pair<string, vector<TagData>>> TagData::GetData(){
	return data;
}
string TagData::GetText(){
	return text;
}

void TagData::AddText(string text){
	this->text += text;
}
