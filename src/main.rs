/*

ToDo:

*/
use std::env;
use std::fs::read_dir;
use id3::{Tag, Version};

fn main(){
    
    let home = std::env::var("HOME").unwrap();

    let mut path: &String =  &(home + "/Music");
    //println!("{}", path);
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        path = &args[1]; 
    }

    //println!("{} {:?}", path, args);

    for entry_res in read_dir(path).unwrap() {
        let entry = entry_res.unwrap();
        let file_name_buf = entry.file_name();
        let file_name = file_name_buf.to_str().unwrap();
        if !file_name.starts_with(".") &&
            entry.file_type().unwrap().is_file() &&
            file_name.ends_with(".mp3")
        {
            println!("{:?}", entry.path());
            let mut name = file_name.to_string();
            name.truncate(name.len() - 4);
            

            let vv: Vec<&str> = name.split(" - ").collect();
            
            //println!("{:?}", vv);
            //println!("File {:?} has full path {:?}",
            //name, entry.path());

            let mut tag = Tag::new();

            let len = vv.len();
            if len == 1 {
                tag.set_title(  vv[0] );
            }else if len == 2 {
                tag.set_artist( vv[0] );
                tag.set_title(  vv[1] );
            }else if len == 3 {
                tag.set_artist( vv[0] );
                tag.set_album(  vv[1] );
                tag.set_title(  vv[2] );
            }else if len == 4 {
                tag.set_artist( vv[0] );
                tag.set_album(  vv[1] );
                tag.set_title(  vv[3] );

                let vvv: Vec<&str> = vv[2].split("|").collect();
                tag.set_track(        vvv[0].parse::<u32>().unwrap() );
                tag.set_total_tracks( vvv[1].parse::<u32>().unwrap() );
            }else{
                println!("WTF")
            }

            tag.write_to_path(entry.path(), Version::Id3v24).unwrap();
        }
    }

}

