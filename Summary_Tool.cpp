//
//  Summary_Tool.cpp
//
//  Created by Prabhdeep Singh Walia on 15/06/15.
//  Copyright (c) 2015 Prabhdeep Singh Walia. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <set>
#include <vector>
#include <map>
using namespace std;

bool comp(string a, string b)
{
    if(a.compare(b)<0)  return true;
    else    return false;
}

class SummaryTool
{
    
private:

    //auxiliary function to split string with given token delimiter
    vector<string> split(const string str, char c = ' ')
    {
        vector<string> result;
        string psh = "";
        for(int i = 0; i<str.size(); i++)
        {
            if(str[i]==c)
            {
                result.push_back(psh);
                psh = "";
            }
            else
            {
                psh+=str[i];
            }
        }
        return result;
    }
    
    //assuming single '.' separating paragraphs
    vector<string> split_into_sentences(string para)
    {
        return split(para, '.');
    }
    
    //assuming single '\n' separating paragraphs
    vector<string> split_into_paragraphs(string para)
    {
        return split(para, '\n');
    }
    
    //token delimiter used ' '
    //calculates a value of similarity between two sentences
    //value = number of common words/average length of the strings
    double sentences_intersection(string sent1, string sent2)
    {
        if(sent1.size()+sent2.size()==0)
            return 0;
        vector<string> a, b, c(400);
        a = split(sent1);
        b = split(sent2);
        sort(a.begin(), a.end(), comp);
        sort(b.begin(), b.end(), comp);
        vector<string>::iterator it;
        it = set_intersection(a.begin(), a.end(), b.begin(), b.end(), c.begin(), comp);
        c.resize(it-c.begin());
        double ret = c.size();
        return (ret/((sent1.size()+(double)sent2.size())/2));
    }
    
    //creates a graph with sentences of a paragraph as nodes
    //each edge weight gives the value of similarity
    //ranking based on total sum of all edges leaving a node
    set< pair<double, string> > get_sentences_rank(const string para)
    {
        vector<string> sent = split_into_sentences(para);   //contains sentences
        int s = sent.size();
        double grph[s][s];
        for(int i = 0; i < s; i++)
        {
            for(int j = 0; j < i; j++)
            {
                grph[i][j] = sentences_intersection(sent[i], sent[j]);
                grph[j][i] = grph[i][j];
            }
        }
        set< pair<double, string> > m;
        for(int i = 0; i < s; i++)
        {
            double score = 0;
            for(int j = 0; j<s; j++)
                score+=grph[i][j];
            m.insert({score, sent[i]});
        }
        return m;
    }
    
    //modify given content to remove non alphabetical characters
    //returns string in accordance with assumptions taken above
    string modify(const string str)
    {
        string ret = "";
        for(int i = 0; i<str.size(); i++)
        {
            if((str[i]>='a' && str[i]<='z') || (str[i]>='A' && str[i]<='Z') || (str[i]=='.') || (str[i]==' ') || (str[i]=='\'') || (str[i]=='-') )
            {
                ret+=str[i];
            }
            else if(str[i] == '\n')
            {
                if(str[i-1]=='\n')  continue;
                else    ret+=str[i];
            }
            else if(str[i]=='?' || str[i]=='!')
            {
                ret+=".";
            }
            else
            {
                ret+=" ";
            }
        }
        return ret;
    }
    
public:
    
    string get_summary(const string title, const string content)
    {
        string cont2 = modify(content);
        string summary = "";
        summary+=title;
        summary+="\n\n";
        vector<string> paras = split_into_paragraphs(cont2);
        for(auto p : paras)
        {
            set<pair<double, string> > best = get_sentences_rank(p);
            set<pair<double, string> >::iterator it = best.end();
            it--;
            summary+=(*it).second;
            summary+=". ";
        }
        return summary;
    }
    
};


int main(int argc, const char * argv[])
{
    SummaryTool A;
    
    //example title and content
    string title = "Break out of cycles of depression, self blame and mediocrity";
    
    
    string content2 = "I have a serious addiction. If you ever talk on the phone with me there’s almost 100% chance I am simultaneously playing chess online. The phone rings and one hand reaches for the phone and the other hand reaches for the computer to initiate a one minute game. Chess rankings are based on a statistically generated rating system. So I can compare easily how well I do when I’m the phone compared with when I’m not on the phone. There is a three standard deviation difference.\nImagine if I were talking on the phone and driving. Or responding to emails. It’s the same thing I’m assuming: phone calls cause a three standard deviation subtraction in intelligence. And that’s the basic multi-tasking we all do at some point or other.\nSo great people can multitask but since, by definition, most of us are not great (99% of us are not in the top 1%), its much better to single-task. Just do one thing at a time. When you wash your hands, hear the sound of the water, feel the water on your hands, scrub every part. Be clean. Focus on what you are doing.\nOften, the successful mediocre entrepreneur should strive for excellence in ZERO-tasking. Do nothing. We always feel like we have to be “doing something” or we (or, I should say “I”) feel ashamed. Sometimes it’s better to just be quiet, to not think of anything at all.";
    
    string summary = A.get_summary(title, content2);
    cout<<summary<<"\n";
    cout<<"\n"<<"Original Length : "<<title.size()+content2.size()<<"\n";
    cout<<"Summary Length : "<<summary.size()<<"\n";
    cout<<"Summary Percentage : "<<100*(1-(summary.size()/((double)title.size()+content2.size())))<<"%\n";
    return 0;
}

