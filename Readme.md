# VHDL Entity Converter - VEC

* Generate schematic symbols in open document format (fodg - open document drawing) from VHDL entity declarations
* Generate SVG files
* Generate DokuWiki table markup code
* Generate Markdown code
* Convert generated fodg file to PNG files

## Example
### Generating a FODG file from a vhdl source file using a custom label and export a DokuWiki table

    vec.exe cntdnmodm_rtl.vhd -l "Countdown" -d

#### Source input

	ENTITY cntdnmodm IS
		GENERIC (
			n : natural := 4;                   -- counter width
			m : natural := 9);                  -- modulo value
		PORT (
			clk_i   : IN  std_ulogic;
			rst_ni  : IN  std_ulogic;
			en_pi   : IN  std_ulogic;
			count_o : OUT std_ulogic_vector(n-1 DOWNTO 0);
			tc_o    : OUT std_ulogic);
	END cntdnmodm;

#### FODG file
![Generated FODG file](http://bwiessneth.github.io/VHDL-entity-converter/cntdnmodm.png "Generated FODG file")

#### DokuWiki markup
	^ Name    ^ Type                 ^  Direction  ^  Polarity  ^ Description ^
	| clk_i   | std_ulogic           |  IN         |  HIGH      |             |
	| rst_ni  | std_ulogic           |  IN         |  LOW       |             |
	| en_pi   | std_ulogic           |  IN         |  HIGH      |             |
	| count_o | std_ulogic_vector[n] |  OUT        |  HIGH      |             |
	| tc_o    | std_ulogic           |  OUT        |  HIGH      |             |


	^ Name ^ Type    ^ Default value ^
	| n    | natural | 4             |
	| m    | natural | 9             |

#### Markdown
Syntax output

    | Name    | Type                 | Direction | Polarity | Description |
    |---------|----------------------|:---------:|:--------:|-------------|
    | clk_i   | std_ulogic           | IN        | HIGH     |             |
    | rst_ni  | std_ulogic           | IN        | LOW      |             |
    | en_pi   | std_ulogic           | IN        | HIGH     |             |
    | count_o | std_ulogic_vector[n] | OUT       | HIGH     |             |
    | tc_o    | std_ulogic           | OUT       | HIGH     |             |
    
    | Name | Type    | Default value |
    |------|---------|---------------|
    | n    | natural | 4             |
    | m    | natural | 9             |

Rendered table (not working with this Gitlab version)

| Name    | Type                 | Direction | Polarity | Description |
|---------|----------------------|:---------:|:--------:|-------------|
| clk_i   | std_ulogic           | IN        | HIGH     |             |
| rst_ni  | std_ulogic           | IN        | LOW      |             |
| en_pi   | std_ulogic           | IN        | HIGH     |             |
| count_o | std_ulogic_vector[n] | OUT       | HIGH     |             |
| tc_o    | std_ulogic           | OUT       | HIGH     |             |

| Name | Type    | Default value |
|------|---------|---------------|
| n    | natural | 4             |
| m    | natural | 9             |

## Usage
#### Windows
Simply drag and drop one or several vhdl-files onto the executable or call the VEC executable with additional parameters from the command line.

#### Unix
Call the VEC executable with parameter.

## Command line arguments

**Note:** Commandline arguments always overwrite settings specified in vec.conf

    -d            Enable DokuWiki export
    -do <PATH>    Specify DokuWiki output path
    -f            Enable FODG export
    -fo <PATH>    Specify FDOG output path
    -m            Enable markdown export
    -mo <PATH>    Specify markdown output path
    -p            Enable PNG export
    -po <PATH>    Specify PNG output path
    -s            Enables SVG export
    -so <PATH>    Specify SVG output path
    -t            Enables LaTeX export
    -to <PATH>    Specify LaTeX output path    
    -l <STRING>   Specify label for the VHDL entity
    -v            Verbose mode
    -dbg <NUMBER> Specify debug level

```<PATH>``` may be absolute or relative.  
```<STRING>``` may contain spaces but must then be wrapped in quotes.



## Customization

VEC will first look for a `vec.conf` file in your current working directory. If it doesn't find anything it will look in `~/.config/vec.conf` and as a last attempt it will try to read the global config at `/etc/vec/vec.conf`

If no `vec.conf` file is found the default values will be used.

Feel free to edit vec.conf to match your needs. The configuration keys are pretty much self-explanatory.

See the Wiki for more information.

## Building

Build information can be found in `INSTALL.md`