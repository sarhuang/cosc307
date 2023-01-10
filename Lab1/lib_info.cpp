/* Name: Sarah Huang
 * Date: 2/6/22
 * Program: lib_info.cpp
 * Purpose: Read in MP3 files and print them out in a specific format.
 *			Artist: # of songs, total time
 *				Album: # of songs, total time
 *					Track#. Song title, Time
 *					Track#. Song title, Time
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

class Song{
	public:
		string songTitle;	//Name of song
		int songTime;		//Time length of song
		int songTrack;		//Track number in album
};

class Album{
	public:
		map <int, Song> songs;	//Collection of songs in album
		string albumName;		//Name of album
		int albumTime;			//Total time length of all songs in album
};

class Artist{
	public:
		map <string, Album> albums;	//Collection of albums done by artist
		string artistName;			//Name of artist
		int artistTime;				//Total time length of all songs done by artist
		int numSongs;				//Total number of songs done by artist
};



//Remove underscores and replace wtih spaces
string rmUnderscores(string s){
	for(unsigned int i = 0; i < s.length(); i++){
		if(s[i] == '_')
			s[i] = ' ';
	}
	return s;
}

//Convert numerical time to string
string trackToString(const int track){
	char time[10];	
	sprintf(time, "%01d:%02d", track/60, track%60);
	return time;
}	

//Convert "m:ss" string to numerical time
int trackToNum(const string track){
	int minutes, seconds;
	sscanf(track.c_str(), "%d:%d", &minutes, &seconds);
	return (minutes * 60) + seconds;
}

//Print Artist in specific format
string printArtist(const string& name, const Artist artist){
	map <string, Album>::const_iterator ait;	//Album map iterator
	map <int, Song>::const_iterator sit;		//Song map iterator
	string solution;							//String to store final output
	string songToString;						//Song object in string form
	string pad(8, ' ');							//Specific padding
	char alb[300];								//Buffer to use springf

	//Print artist
	sprintf(alb, "%s: %d, %s", name.c_str(), artist.numSongs, trackToString(artist.artistTime).c_str());
	solution += alb;

	for(ait = artist.albums.begin(); ait != artist.albums.end(); ait++){
		//Print album
		sprintf(alb, "\n%s%s: %lu, %s", pad.c_str(), ait->first.c_str(), ait->second.songs.size(), trackToString(ait->second.albumTime).c_str());
		solution += alb;

		//Print all songs in album
		for(sit = ait->second.songs.begin(); sit != ait->second.songs.end(); sit++){
			songToString = sit->second.songTitle + ": " + trackToString(sit->second.songTime);			
			sprintf(alb, "\n%s%s%d. %s", pad.c_str(), pad.c_str(), sit->first, songToString.c_str());
			solution += alb;
		}
	}
	return solution;
}






int main(int argc, char *argv[]){
	//ALL VARIABLES
	ifstream ifs;								//For reading in files					
	map <string, Artist> allArtists;			//Map of all artists
	map <string, Artist>::const_iterator artit;	//Artist map iterator
	string title;								//Title of the song
	string duration;							//Time of the song
	string artist;								//Name of the artist
	string album;								//Name of the album
	string genre;								//Genre of the song
	int track;									//Track number
	Album* tempAlbum;							//Placeholder of Album
	Song tempSong;								//Placeholder of Song
	string solution;							//String placeholder for final solution

	//If bad command line
	if(argc != 2){
		printf("The format should be ./lib_info file\n");
		return -1;
	}

	//Read in songs
	ifs.open(argv[1]);
	while(ifs >> title >> duration >> artist >> album >> genre >> track){
		//Add Artist in map and associate album to artist
		artist = rmUnderscores(artist);
		album = rmUnderscores(album);
		tempAlbum = &allArtists[artist].albums[album];
		
		//Add song to map
		tempSong.songTitle = rmUnderscores(title);
		tempSong.songTime = trackToNum(duration);
		tempSong.songTrack = track;
		tempAlbum->songs.insert(pair <int, Song> (track, tempSong));

		//Update album and artist information
		tempAlbum->albumTime += tempAlbum->songs.at(track).songTime;
		allArtists[rmUnderscores(artist)].artistTime += tempAlbum->songs.at(track).songTime;
		allArtists[rmUnderscores(artist)].numSongs++;
	}

	//Print all artists
	for(artit = allArtists.begin(); artit != allArtists.end(); artit++){
		solution += printArtist(artit->first, artit->second) + "\n";
	}
	printf("%s", solution.c_str());

	ifs.close();
	return 0;
}
