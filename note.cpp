note.cpp
const int cols[4] = {26, 19, 13, 6};
const int raws[4] = {21, 20, 16, 12};
void init_gpio(const int* c, const int* r)
{
	gpiod::chip chip("gpiochip0");
	for(int i=0; i<4; i++)
	{
		gpiod::line line = chip.get_line(c[i]);
		line.request({"matrix_keyboard_col", gpiod::line_request::DIRECTION_OUTPUT, 0});
	}
	for(int i=0; i<4; i++)
	{
		gpiod::line line = chip.get_line(r[i]);
		line.request({"matrix_keyboard_row", gpiod::line_request::DIRECTION_INPUT});
	}
}
int main() {
 gpiod::chip chip("gpiochip0");
 init_gpio(cols, raws);
 std::vector<gpiod::line> col_io, raw_io;
 for(int i=0; i<4; i++)
 {
		col_io.push_back(chip.get_line(cols[i]));
		raw_io.push_back(chip.get_line(raws[i]));
	}
	std::cout << "group OK" << std::endl;
	while(1)
	{
		for(int m=0; m<4; m++)
		{
			try
			{
			col_io[m].set_value(1);
		}catch (const std::exception& e) {
 std::cerr << " GPIO fail: " << e.what() << std::endl;}
			for(int n=0; n<4; n++)
			{
				if(raw_io[n].get_value()==1)
				{
					std::cout << "Key" << n << m << std::endl;
					while(raw_io[n].get_value())
					{
						delay_ms(10);
					}
				}
			}
		}
	}
}
