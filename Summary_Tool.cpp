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
    string title = "Swayy is a beautiful new dashboard for discovering and curating online content";
    string content = "Bonus points if you can name the computer game, board game, or book that inspired each of those thoughts. So, why aren't you reading those books and playing those games yourself? All things come to he who researches. Played RoboRally? Is the principle of pre-programming a sequence of moves applicable to multiplayer Tetris? Or that jumping-pegs thing? Or a colonize-the-galaxy game?\nLast week I bought a set of those colored one-centimeter rods that they use to teach first-grade math. I haven't decided what to do with them yet. Maybe build a tower, or push them around the table like little trains. If it's enough fun, it's worth designing a game. Did you play in the mud when you were a kid?  Ever turn the garden hose on a sandcastle and watch it melt? If so, is there a computer game like that? If not, what's wrong with you?\n";
    
    string content2 = "Lior Degani, the Co-Founder and head of Marketing of Swayy, pinged me last week when I was in California to tell me about his startup and give me beta access. I heard his pitch and was skeptical. I was also tired, cranky and missing my kids – so my frame of mind wasn't the most positive. I went into Swayy to check it out, and when it asked for access to my Twitter and permission to tweet from my account, all I could think was, “If this thing spams my Twitter account I am going to bitch-slap him all over the Internet.” Fortunately that thought stayed in my head, and not out of my mouth.\nOne week later, I'm totally addicted to Swayy and glad I said nothing about the spam (it doesn't send out spam tweets but I liked the line too much to not use it for this article). I pinged Lior on Facebook with a request for a beta access code for TNW readers. I also asked how soon can I write about it. It's that good. Seriously. I use every content curation service online. It really is That Good.\nWhat is Swayy? It's like Percolate and LinkedIn recommended articles, mixed with trending keywords for the topics you find interesting, combined with an analytics dashboard that shows the trends of what you do and how people react to it. I like it for the simplicity and accuracy of the content curation. Everything I'm actually interested in reading is in one place – I don’t have to skip from another major tech blog over to Harvard Business Review then hop over to another major tech or business blog. It's all in there. And it has saved me So Much Time.\nAfter I decided that I trusted the service, I added my Facebook and LinkedIn accounts. The content just got That Much Better. I can share from the service itself, but I generally prefer reading the actual post first – so I end up sharing it from the main link, using Swayy more as a service for discovery.\nI'm also finding myself checking out trending keywords more often (more often than never, which is how often I do it on Twitter.com).\nThe analytics side isn't as interesting for me right now, but that could be due to the fact that I've barely been online since I came back from the US last weekend. The graphs also haven't given me any particularly special insights as I can't see which post got the actual feedback on the graph side (however there are numbers on the Timeline side.) This is a Beta though, and new features are being added and improved daily. I'm sure this is on the list. As they say, if you aren’t launching with something you're embarrassed by, you’ve waited too long to launch.\nIt was the suggested content that impressed me the most. The articles really are spot on – which is why I pinged Lior again to ask a few questions:\nHow do you choose the articles listed on the site? Is there an algorithm involved? And is there any IP?\nYes, we're in the process of filing a patent for it. But basically the system works with a Natural Language Processing Engine. Actually, there are several parts for the content matching, but besides analyzing what topics the articles are talking about, we have machine learning algorithms that match you to the relevant suggested stuff. For example, if you shared an article about Zuck that got a good reaction from your followers, we might offer you another one about Kevin Systrom (just a simple example).\nWho came up with the idea for Swayy, and why? And what's your business model?\nOur business model is a subscription model for extra social accounts (extra Facebook / Twitter, etc) and team collaboration.\nThe idea was born from our day-to-day need to be active on social media, look for the best content to share with our followers, grow them, and measure what content works best.\nWho is on the team?\nOhad Frankfurt is the CEO, Shlomi Babluki is the CTO and Oz Katz does Product and Engineering, and I [Lior Degani] do Marketing. The four of us are the founders. Oz and I were in 8200 [an elite Israeli army unit] together. Emily Engelson does Community Management and Graphic Design.\nIf you use Percolate or read LinkedIn's recommended posts I think you'll love Swayy.";
    
    string summary = A.get_summary(title, content2);
    cout<<summary<<"\n";
    cout<<"\n"<<"Original Length : "<<title.size()+content2.size()<<"\n";
    cout<<"Summary Length : "<<summary.size()<<"\n";
    cout<<"Summary Percentage : "<<100*(1-(summary.size()/((double)title.size()+content2.size())))<<"%\n";
    return 0;
}

