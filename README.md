# XFBIN-JSON Converter & Merger - for All-Star Battle R
Current supported ASBR file types are:
- messageInfo
More coming soon.

## Conversion
To convert a file **to JSON or back**, simply _drag_ the file you want onto the EXE.
Ensure that the formatting is correct for the XFBIN or JSON, as incorrect formatting will cause errors. Feel free to report any errors you do come across, and I'll add error detection for those specific things in future releases.

## Merging
To **merge JSON files**, _open_ the EXE and input (text) the type of file you want to merge.

You will need to have prepared a folder within the `merging` folder with the name of your wanted data type (e.g. `merging/messageInfo`).

Within that folder, have the JSON files you want to merge with the original (original or "base" JSON is already included in the `data` folder) alongside a corresponding TXT file of the same name. For example, `apple.json` would be accompanied by `apple.txt`. Inside these JSON files, you only need to include what you want to modify:

```json
{
    "1c30e109": {
    "message": "bro just got violated",
    "is_ref": -1,
    "ref_crc32": "",
    "char_id": 70,
    "cue_id": 84
  },
    "6416af04": {
    "message": "woohoo yippee",
    "is_ref": 1,
    "ref_crc32": "c37860ee",
    "char_id": 70,
    "cue_id": 83
  }
}
```
^ An example for `messageInfo`

Within these TXTs should be a **priority integer**. This determines the order in which the files are merged, which is useful for dealing with files that try to modify the same values. **Higher number = higher priority (merges after others)**. For example, `apple.json` with priority `5` will override `pear.json` with priority `2`.

