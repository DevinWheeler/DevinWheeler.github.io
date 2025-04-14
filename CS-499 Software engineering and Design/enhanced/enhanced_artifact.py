import tkinter as tk
from tkinter import messagebox

class InvestmentCalculator:
    def __init__(self, root):
        self.root = root
        self.root.title("Banking Investment Calculator")
        self.root.geometry("500x550")
        self.root.configure(bg="#f0f0f0")

        # Title label
        title_label = tk.Label(self.root, text="Banking Investment Calculator", font=("Arial", 16, "bold"), bg="#4CAF50", fg="white")
        title_label.grid(row=0, column=0, columnspan=2, pady=10)

        # Input fields
        self.create_input_field("Initial Investment Amount:", 1)
        self.create_input_field("Monthly Deposit:", 2)
        self.create_input_field("Annual Interest Rate (%):", 3)
        self.create_input_field("Number of Years:", 4)

        # Compounding frequency dropdown
        label = tk.Label(self.root, text="Compounding Frequency:", bg="#f0f0f0", fg="black")
        label.grid(row=5, column=0, padx=10, pady=5, sticky="w")
        self.compounding_var = tk.StringVar(value="Monthly")
        self.compounding_options = ["Monthly", "Quarterly", "Semi-Annually", "Annually"]
        dropdown = tk.OptionMenu(self.root, self.compounding_var, *self.compounding_options)
        dropdown.config(bg="#e6e6e6", fg="black", highlightbackground="#e6e6e6")
        dropdown.grid(row=5, column=1, padx=10, pady=5)

        # Calculate button
        calculate_btn = tk.Button(self.root, text="Calculate", command=self.process_user_input, bg="#2196F3", fg="white", padx=20, pady=5)
        calculate_btn.grid(row=6, column=0, columnspan=2, pady=10)

        # Output text box
        self.output = tk.Text(self.root, width=60, height=15, bg="#e6e6e6", fg="black", font=("Courier", 10))
        self.output.grid(row=7, column=0, columnspan=2, padx=10, pady=10)

    # Helper function to create inputs with labels
    def create_input_field(self, label_text, row):
    
        label = tk.Label(self.root, text=label_text, bg="#f0f0f0", fg="black")
        label.grid(row=row, column=0, padx=10, pady=5, sticky="w")
        entry = tk.Entry(self.root)
        entry.grid(row=row, column=1, padx=10, pady=5)
        setattr(self, f'entry_{row}', entry)

    # Validates the users inputs
    def process_user_input(self):
        try:
            initial_investment = float(self.entry_1.get())
            monthly_deposit = float(self.entry_2.get())
            annual_interest = float(self.entry_3.get())
            years = int(self.entry_4.get())
            frequency = self.compounding_var.get()

            # Validate inputs to ensure they are non-negative
            if initial_investment < 0 or monthly_deposit < 0 or annual_interest < 0 or years <= 0:
                raise ValueError("Negative values or zero not allowed.")

            # Clear output before displaying new results
            self.output.delete(1.0, tk.END)

            # Perform calculations and display results
            self.display_investment_report(initial_investment, monthly_deposit, annual_interest, years, frequency)
        except ValueError:
            # Display error message
            messagebox.showerror("Invalid Input", "Please enter valid positive numeric values. Years must be greater than zero.")

    # Function to get the number of compounding periods
    def get_compounding_periods(self, frequency):
        return {
            "Monthly": 12,
            "Quarterly": 4,
            "Semi-Annually": 2,
            "Annually": 1
        }[frequency]

    # Function to calculate the investment report
    def calculate_balance(self, initial_investment, monthly_deposit, annual_interest, years, frequency):
        current_balance = initial_investment
        compounding_periods = self.get_compounding_periods(frequency)
        total_interest = 0

        for year in range(1, years + 1):
            yearly_interest = 0

            for month in range(12):
                # Add monthly deposit at the beginning of the month
                current_balance += monthly_deposit

                # Apply interest at the end of each compounding period
                if (month + 1) % (12 // compounding_periods) == 0:
                    interest = current_balance * (annual_interest / 100) / compounding_periods
                    yearly_interest += interest
                    current_balance += interest

            total_interest += yearly_interest
            yield year, current_balance, yearly_interest, total_interest

# Function to display the investment report
    def display_investment_report(self, initial_investment, monthly_deposit, annual_interest, years, frequency):
        tax_rate = 0.25  # Assuming 25% tax rate for this example
        self.output.insert(tk.END, f"Year-End Balances with {frequency} Compounding:\n")
        self.output.insert(tk.END, f"{'Year':<10}{'Balance':>20}{'Interest Earned':>20}\n")
        self.output.insert(tk.END, f"{'-' * 50}\n")

        for year, balance, interest, total_interest in self.calculate_balance(initial_investment, monthly_deposit, annual_interest, years, frequency):
            self.output.insert(tk.END, f"{year:<10}${balance:>18,.2f}${interest:>18,.2f}\n")

        tax_due = total_interest * tax_rate
        self.output.insert(tk.END, f"\nTotal Interest Earned: ${total_interest:,.2f}\n")
        self.output.insert(tk.END, f"Estimated Tax Due (25%): ${tax_due:,.2f}\n")

if __name__ == "__main__":
    root = tk.Tk()
    app = InvestmentCalculator(root)
    root.mainloop()
