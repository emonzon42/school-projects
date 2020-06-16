import java.util.*;
import javax.swing.JOptionPane; 

public class LinkedSetDriver {
    public static void main(String[] args) {
        int limit = 1;
        boolean error = true;
        boolean itemFound;
        String last = "";
        LinkedSet<String> set = new LinkedSet<>();

        while (error) {
            try {
                limit = Integer.parseInt(JOptionPane.showInputDialog(null, "How many items do you want in your set?", "Eli's Program", 1));
                error = false;
            } catch (NumberFormatException e) {
                error = true;
                JOptionPane.showMessageDialog(null, "That was an invalid number try again!: ", "ERROR", 0);
            }
        }
        for (int i = 0; i < limit; ++i) {
            last = JOptionPane.showInputDialog(null, "Okay now add items to your set: ", "Eli's Program", 1);
            boolean uno = set.add(last);
            if (!uno)
                JOptionPane.showMessageDialog(null,"A set cannot have the same item more than once, skipping...", "Eli's Program", 2);
        }

        JOptionPane.showMessageDialog(null, "Your set looks like this so far: " + set.toString(), "Eli's Program", 1);
        JOptionPane.showMessageDialog(null, "Now I will remove the last thing you added...", "Eli's Program", 1);
        set.remove(last);
        JOptionPane.showMessageDialog(null, "This is your set now: " + set.toString(), "Eli's Program", 1);

        itemFound = set.contains(JOptionPane.showInputDialog(null, "Type something and we'll check if your set contains it: ", "Eli's Program", 1));
        if (itemFound) {
            JOptionPane.showMessageDialog(null, "Your set contains that item", "Eli's Program", 1);
        } else {
            JOptionPane.showMessageDialog(null, "Your set doesn't contain that item", "Eli's Program", 2);
        }

        JOptionPane.showMessageDialog(null, "The size of your set is: " + set.size(), "Eli's Program", 1);

        JOptionPane.showMessageDialog(null, "It is " + set.isEmpty() + " that your set is empty : " + set, "Eli's Program", 1);
        JOptionPane.showMessageDialog(null, "I am now going to clear your set...", "Eli's Program", 2);
        set.clear();
        JOptionPane.showMessageDialog(null, "It is now " + set.isEmpty() + " that your set is empty : " + set, "Eli's Program", 1);

    }
}