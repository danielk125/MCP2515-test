import cantools
import os
import pandas as pd
from can_parser import CANDatabase
    
def generate_signals(db_file, output_file):
    df = pd.read_csv(db_file)
    df = df.dropna(subset=['Message Name', 'Start Bit', 'Size (bits)', 'Factor', 'Offset'])
    lists = [list(df.columns), *df.values.tolist()]
    db = CANDatabase(lists)

    buses = db.get_buses()
    messages_dict = db.get_messages()
    with open(output_file, "w") as f:
        for message in messages_dict.values():
            signals = message.get_signals()
            can_msg_str = f"CAN_MESSAGE<{len(signals)}> {{{list(message.buses)[0]}, {message.message_id}"
            for signal in signals:
                signal_str = signal.as_cpp_code()
                f.write(signal_str + "\n")
                can_msg_str += ", " + signal.signal_name
            can_msg_str += "}};"
            f.write(can_msg_str + "\n\n")
        # write string
        # Make CANMessage array with specific size, name, bus, id, and all the signals
        

        

generate_signals("can_dbc.csv", "can_signals.cpp")

