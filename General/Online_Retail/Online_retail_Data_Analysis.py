
from ucimlrepo import fetch_ucirepo 
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt  
import datetime
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import classification_report, confusion_matrix
from sklearn.preprocessing import StandardScaler

from sklearn.cluster import KMeans

# fetch dataset 
online_retail = fetch_ucirepo(id=352) 
  

data = pd.DataFrame(online_retail.data.original)

data.describe()

data.columns

#Create revenue per product
data["product_revenue"] = data["Quantity"]*data["UnitPrice"]

#group by countries to see the top 10 countries 
s_data_country = data.groupby("Country").agg({"Quantity": "sum", "UnitPrice":"mean", "product_revenue": "sum"}).sort_values("Quantity", ascending=False)

sns.barplot(data = s_data_country.head(10), y = "Country", x = "Quantity",  palette="viridis")
plt.tight_layout()

sns.barplot(data = s_data_country.head(10).sort_values("UnitPrice", ascending = False), y = "Country", x = "UnitPrice",  palette="viridis")
plt.show()

s_data_product = data.groupby("Description").sum("Quantity").sort_values("product_revenue", ascending = False)
plt.show()

b_w_products = pd.concat([s_data_product.head(10).sort_values("product_revenue", ascending = False),s_data_product.tail(5).sort_values("product_revenue", ascending = False)], axis=0)

sns.barplot(data = b_w_products, y = "Description", x = "product_revenue",  palette="viridis")
plt.show()

grouped_df = data.groupby(["Description", "Country"]).sum("product_revenue").reset_index()

products_sales = grouped_df.sort_values(["Country", "product_revenue"], ascending = [True, False])

top_products_per_country = products_sales.drop_duplicates(subset=["Country"], keep="first").sort_values("product_revenue", ascending = False)

top_5_country_rev = top_products_per_country[["Description", "Country", "product_revenue"]].head(5)


sns.barplot(data = top_5_country_rev, y = "Country", x = "product_revenue",  palette="viridis")
plt.show()


top_customers = data.groupby("CustomerID")["product_revenue"].sum().sort_values(ascending=False).head(5).reset_index()

# Convert 'InvoiceDate' to datetime if available
data['InvoiceDate'] = pd.to_datetime(data['InvoiceDate'])

snapshot_date = data['InvoiceDate'].max() + pd.Timedelta(days=1)

rfm = data.groupby('CustomerID').agg({
    'InvoiceDate': lambda x: (snapshot_date - x.max()).days,  # Recency
    'InvoiceNo': 'nunique',                                   # Frequency
    'product_revenue': 'sum'                                  # Monetary
}).rename(columns={'InvoiceDate': 'Recency', 'InvoiceNo': 'Frequency'})

rfm.sort_values("product_revenue", ascending = False).head(5)


data["InvoiceDate"] = pd.to_datetime(data["InvoiceDate"])
data["YearMonth"] = data["InvoiceDate"].dt.to_period("M")
monthly_revenue = data.groupby("YearMonth")["product_revenue"].sum().reset_index()
#monthly_revenue["YearMonth"] = monthly_revenue["YearMonth"].astype(str)  # convert Period to string for plotting

# Plot as barplot
plt.figure(figsize=(12, 6))
sns.barplot(data=monthly_revenue, x="YearMonth", y="product_revenue", palette="viridis")
plt.title("Monthly Revenue")
plt.xticks(rotation=45, ha="right")
plt.ylabel("Revenue")
plt.xlabel("Month")
plt.tight_layout()
plt.show()

# Convert InvoiceDate to datetime and extract year-month
data["InvoiceDate"] = pd.to_datetime(data["InvoiceDate"])
data["YearMonth"] = data["InvoiceDate"].dt.to_period("M").astype(str)

# Get top 10 products by quantity sold
top_products  = data.groupby("Description")["Quantity"].sum().sort_values(ascending=False).head(10).index
top_countries = data.groupby("Country")["Quantity"].sum().sort_values(ascending=False).head(5).index



 # Filter data to top products and countries
filtered_data = data[
(data["Description"].isin(top_products)) &
(data["Country"].isin(top_countries))
]

def top_prod_plot(filter_by,col):  
    # Pivot table: Products vs. Months
    heatmap_data_1 = filtered_data.pivot_table(index="Description", 
                                                 columns=col, 
                                                 values="Quantity", 
                                                 aggfunc="sum").fillna(0)
    
    # Plot heatmap
    plt.figure(figsize=(14, 6))
    sns.heatmap(heatmap_data_1, cmap="YlGnBu", annot=True, fmt=".0f")
    plt.title("Heatmap of Top 10 Product Sales by " + col)
    plt.ylabel("Product Description")
    plt.xlabel(col)
    plt.xticks(rotation=45)
    plt.tight_layout()
    plt.show()


#plot top products for Year of month
top_prod_plot(filter_by = "Description", col = "YearMonth")
#plot top products for Country
top_prod_plot(filter_by = "Description", col = "Country")

# Group revenue per invoice per country
invoice_revenue = data.groupby(["InvoiceNo", "Country"])["product_revenue"].sum().reset_index()

# Plot boxplot
plt.figure(figsize=(14, 6))
sns.boxplot(data=invoice_revenue, x="Country", y="product_revenue")
plt.xticks(rotation=90)
plt.title("Distribution of Invoice Revenue by Country")
plt.ylabel("Invoice Revenue")
plt.xlabel("Country")
plt.tight_layout()
plt.show()


# Define return as negative quantity
data["is_return"] = data["Quantity"] < 0

# Drop rows with missing values
data_clean = data.dropna(subset=["CustomerID", "Description", "InvoiceDate", "Quantity", "UnitPrice"]).copy()

# Remove zero or negative prices (keep only meaningful sales)
data_clean = data_clean[data_clean["UnitPrice"] > 0]

# Convert date to datetime
data_clean["InvoiceDate"] = pd.to_datetime(data_clean["InvoiceDate"])

# Create additional features
data_clean["InvoiceMonth"] = data_clean["InvoiceDate"].dt.month
data_clean["InvoiceHour"] = data_clean["InvoiceDate"].dt.hour
data_clean["is_weekend"] = data_clean["InvoiceDate"].dt.weekday >= 5
data_clean["TotalAmount"] = data_clean["Quantity"] * data_clean["UnitPrice"]

# Sort by date (useful if doing time-based splits later)
data_clean = data_clean.sort_values("InvoiceDate")

data_clean.columns

# --- Step 2: Feature Set ---
feature_cols = ["UnitPrice",  "TotalAmount", 'Quantity',  "InvoiceMonth",  "InvoiceHour",  "is_weekend"]
X = data_clean[feature_cols]
y = data_clean["is_return"]

# --- Step 3: Train/Test Split with Stratification ---
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.3, random_state=42, stratify=y
)

# --- Step 4: Scaling ---
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# --- Step 5: Train Logistic Regression Model ---
model = LogisticRegression(max_iter=1000)
model.fit(X_train_scaled, y_train)

# --- Step 6: Predictions ---
y_pred = model.predict(X_test_scaled)
y_probs = model.predict_proba(X_test_scaled)[:, 1]  # Probabilities of return

# --- Step 7: Evaluation ---
print("Classification Report:\n", classification_report(y_test, y_pred))

# Confusion Matrix
cm = confusion_matrix(y_test, y_pred)
sns.heatmap(cm, annot=True, fmt="d", cmap="Blues", xticklabels=["No Return", "Return"], yticklabels=["No Return", "Return"])
plt.title("Confusion Matrix")
plt.xlabel("Predicted")
plt.ylabel("Actual")
plt.show()

# --- Step 8: Visualize Feature Importance ---
coefficients = pd.DataFrame({
    "Feature": feature_cols,
    "Coefficient": model.coef_[0]
}).sort_values("Coefficient", ascending=False)

sns.barplot(x="Coefficient", y="Feature", data=coefficients, palette="coolwarm")
plt.title("Logistic Regression Coefficients")
plt.axvline(0, color="black", linestyle="--")
plt.tight_layout()
plt.show()

# --- Step 9: Attach Probabilities Back for Future Analysis ---
X_test_with_prob = X_test.copy()
X_test_with_prob["return_prob"] = y_probs
X_test_with_prob["actual"] = y_test.values


y_probs = model.predict_proba(X_test)[:, 1]
data_clean.loc[X_test.index, "return_prob"] = y_probs
top_products = (
    data_clean.loc[X_test.index]
    .groupby("Description")["return_prob"]
    .mean()
    .sort_values(ascending=False)
    .head(30)
)

plt.figure(figsize=(10, 5))
sns.barplot(x=top_products.values, y=top_products.index, palette="Reds_r")
plt.xlabel("Avg Return Probability")
plt.title("Top 10 Products by Predicted Return Likelihood")
plt.tight_layout()
plt.show()
